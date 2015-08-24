// 		DESCRIZIONE
/*
 -- INTRODUZIONE

 -- BYPASS
Gli interruttori si chiamano DIP e la libreria per usarli è "dsk5510_dip.h".
La variabile che contiene lo stato dell'interruttore 'i' è DSK5510_DIP_get(i); i = 0..3.


 -- EXTRA
La selezione periodica dalla Lut agisce tipo phaser.
Unito al distorsore, fa un effetto simpatico.


 *  The 5510 DSK Board Support Library is divided into several modules, each
 *  of which has its own include file.  The file dsk5510.h must be included
 *  in every program that uses the BSL.  This example also includes
 *  dsk5510_led.h  because it uses the LED module.
 *  
 *  
 */



/*****************************************************************************/
/*   include								     */
/*****************************************************************************/

#include "ledcfg.h"
#include "dsk5510.h"
#include "dsk5510_led.h"
#include "dsk5510_dip.h"
#include "dsk5510_aic23.h"
#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_timer.h>
#include "stdio.h"
#include "dsk5510.h"
#include "TMS320.h"
#include "Dsplib.h"

Uint32 freq;
#include "CONTROLLI.H"
#include "lut.h"
#include "filtro.h"


/*****************************************************************************/
/*   define																     */
/*****************************************************************************/

/*
//level values
#define LEVEL_0dB  0x4000
#define LEVEL_3dB  0x2D3F
#define LEVEL_6dB  0x2000
#define LEVEL_9dB  0x169F
#define LEVEL_12dB 0x1000
#define LEVEL_18dB 0x0800
#define LEVEL_24dB 0x0400
#define LEVEL_36dB 0x0100
*/

/* Codec configuration settings */
DSK5510_AIC23_Config config = {	// usiamo la configurazione trovata in "test_aic"
    0x0017,  /* 0 DSK5510_AIC23_LEFTINVOL  Left line input channel volume */ \
    0x0017,  /* 1 DSK5510_AIC23_RIGHTINVOL Right line input channel volume */\
    0x01f9,  /* 2 DSK5510_AIC23_LEFTHPVOL  Left channel headphone volume */  \
    0x01f9,  /* 3 DSK5510_AIC23_RIGHTHPVOL Right channel headphone volume */ \
    0x0015,  /* 4 DSK5510_AIC23_ANAPATH    Analog audio path control */      \
    0x0000,  /* 5 DSK5510_AIC23_DIGPATH    Digital audio path control */     \
    0x0000,  /* 6 DSK5510_AIC23_POWERDOWN  Power down control */             \
    0x0043,  /* 7 DSK5510_AIC23_DIGIF      Digital audio interface format */ \
    0x0081,  /* 8 DSK5510_AIC23_SAMPLERATE Sample rate control */            \
    0x0001   /* 9 DSK5510_AIC23_DIGACT     Digital interface activation */   \
};

//	Descrizione delle impostazioni di configurazione del codec
/*
 ind  	registri in binario	  descrizione
0000.	0000 0000 0001 0111	mute: disabled; volume: default
0001.	0000 0000 0001 0111	idem a destra
0010.	0000 0001 1111 1001	simultaneous update; volume: default
0011.	0000 0001 1111 1001	idem a destra
0100.	0000 0000 0001 0101	mic boost; mic sel; dis: byp, mic mute, sidetone; dac sel
0101.	0000 0000 0000 0000	dac softmute: disabled
0110.	0000 0000 0000 0000	power down ON (ma cosa fa?)
0111.	0000 0000 0100 0011	data format: DSP; mode: master
1000.	0000 0000 1000 0001	clk mode: USB; Sampl rate: v.3.3.2.; clkout=mclk/2
1001.	0000 0000 0000 0001	activate interface
*/
/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;



/*****************************************************************************/
/* main()                                                                    */
/*****************************************************************************/
float cerca;

void main(void) {

    DSK5510_AIC23_CodecHandle hCodec;
    Uint32 delay, delay1, i;


	/* Initialize the board support library, must be called first */
    DSK5510_init();
	/* Initialize the LED modules of the BSL */
    DSK5510_LED_init();
	      
    for (delay = 0; delay < 10; delay++) {	// facciamo lampeggiare un po' di led
		DSK5510_LED_toggle(1);

        /* Software delay loop, about 200ms */
		for (delay1 = 0; delay1 < 3000000; delay1++);

    } // lampeggini di avviamento

    
/*	Start the codec */
	hCodec = DSK5510_AIC23_openCodec(0, &config);  
    
//	freq=1;	//  8kHz
//	freq=2; // 16kHz
	freq=3; // 24kHz
//	freq=5;	// 44kHz
//	freq=6; // 48kHz
/*	nb: 1-7, v. dsk5510_aic23.h ----- freq=6 (=48 kHz) e' il default */
    DSK5510_AIC23_setFreq(hCodec, freq);

	inizNuovaLut(GUADAGNO);
	preparaFiltro(freq, RC);
	



	while(1) {	// ciclo infinito

    /* Lettura */
     while (!DSK5510_AIC23_read16(hCodec, &left_input ));	
     while (!DSK5510_AIC23_read16(hCodec, &right_input));	
     


    /* Elaborazione */
	ing[0] = GUADAGNO * left_input;
	corr=1; // fattore correttivo del volume

	if(dis) {
		ing[0] = ing[0]*DISTO;
		if(ing[0] > 32000) ing[0] = 32000;
		if(ing[0] < -32000) ing[0] = -32000;
		ing[0] = ing[0]/DISTO*100;
		corr = corr/3;
	}
	
	somma = 0;
	rad[nuo] = abs(ing[0]);

	prod = b1*rad[nuo];
	somma+=prod; prod = b0*rad[vec];
	somma+=prod; prod = a0*inv[vec];
	somma+=prod; prod = AA*ing[0];
	inv[nuo] = somma; somma=0;

	for (i = 0; i < 4; i++)
		if (inv[nuo] > 1200*GUADAGNO*(i+1))
			DSK5510_LED_on(i);
		else
			DSK5510_LED_off(i);


	if (wah | pha) {


		if (wah) {					/* wah wah vero e proprio */
			c = NuovaLut(cerca);	//
		} else {					/* wah periodico */
			c = perLut();			//
		}	


		A2c = A2*c;
	
		somma+=prod; prod = mAA*ing[2];
		somma+=prod; prod = A2c*y[1];
		somma+=prod; prod = mAA*y[2];
		somma+=prod;

		y[0] = somma;
				
		ing[2]=ing[1];
		ing[1]=ing[0];

		y[2]=y[1];
		y[1]=y[0];

		corr = corr*0.2;
	} else {
		y[0] = ing[0];
	}
	
	if(vec) vec=0; else vec=1;
	if(nuo) nuo=0; else nuo=1;

	
	wah = tastino(1);	//
	pha = tastino(2);	//	per far comandare l'effetto dagl'interruttori
	dis = tastino(3);	//


	left_output=y[0]*corr*VOLUME;
	right_output=left_output;	      
    /* Scrittura di un campione per canale */
	while (!DSK5510_AIC23_write16(hCodec, left_output ));	// NB: sono alla        
	while (!DSK5510_AIC23_write16(hCodec, right_output));	//	rovescia
//	NB ci vogliono entrambi i write, altrimenti va il doppio piu' veloce
/*	nb *non* e' corretto invece mettere DSK5510_AIC23_write16(hCodec, val); */   
      

   } // ciclo infinito

    /* Close the codec */
//	DSK5510_AIC23_closeCodec(hCodec);
//	chiaramente NON si arriverà MAI a questo punto

} //MAIN





/********* LUT *********/

void inizNuovaLut(float GUADAGNO){
	int i;
	for(i=0; i<NN; i++) {
		xx[i]=(xx[i]+2500*GUADAGNO/2);
	}
	x0 = xx[0];
	contropasso = 1/(xx[1]-xx[0]);
} // inizNuovaLut

//	NUOVALUT
float NuovaLut(float val) {
	int indice;

	if (val < xx[0]) {
		return 1.0;
	}
	if (val > xx[NN-1]) {
		return 0.950390;
	}
	indice = (int)((val - x0)*contropasso);
	return yy[indice];
} //	NuovaLut

float perLut() {
	if(LutCNT==1000) {
		if(LutIND==30)	inc =-1;
		if(LutIND==0)	inc = 1;
		LutIND+=inc;
		LutCNT=0;
	}
	LutCNT++;
	return yy[LutIND];
} //	perLut






/****** FILTRO ******/

void preparaFiltro(int freq, float RC) {

//	Coefficienti
	float freq_camp = 3000 * freq,
		T = 1/freq_camp;

	A = 0.9;							// Passabanda
	AA=A*A; A2=A*2; mAA=-AA;

	b1 = T/(2*RC+T), b0 = b1,			// Passabasso
	a1 = 1, a0 = (2*RC-T)/(2*RC+T);

	c=0; A2c=0;

	vec=0; nuo=1;

	somma=0; prod=0;

	dip_buf_len=3; dip_cnt=0; //tutti1=0; tutti0=0;
	acceso=0; tmp=0;

	inizVett();
}	// preparaFiltro


void inizVett() {
	int i;
	for (i=0; i<3; i++) {
		ing[i]=0;
		y[i]=0;
	}
	for (i=0; i<2; i++) {
		rad[i]=0;
		inv[i]=0;
	}
	for (i=0; i<dip_buf_len; i++) {
		dip_buf[i]=0;
	}
} // inizVett



int tastino(int dip_num) {
	int dip = DSK5510_DIP_get(dip_num);
	if (!dip) tmp=1;
	if (dip && tmp) {
		if(acceso) acceso=0; else acceso=1;
		tmp=0;
	}
	return acceso;
} // tastino
