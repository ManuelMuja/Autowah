/*
 *  lut.h
 *  Simulwah
 *
 *  Created by Manuel Iurissevich on 04/12/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *	
 *	La NuovaLut usa la seconda tavola ma effettua una
 *	ricerca "a colpo sicuro": visto che la differenza x[i]-x[i-1]
 *	è costante (per i = 1, ..., 38), l'indice cercato si può ottenere
 *	con una due semplici operazioni: indice = parteIntera(xc - x[0]) / passo);
 *	dove passo = x[1]-x[0].
 *	Per rendere i calcoli ancora più semplici per il processore,
 *	conviene memorizzare x0 = -_x[0] e contropasso = 1/passo in fase di inizializzazione
 *	in maniera che durante l'elaborazione si eseguano una somma e una moltiplicazione
 *	più immediate rispetto a sottrazione e divisione.
 *	ANCORA MEGLIO SAREBBE EFFETTUARE UNO _SCORRIMENTO A SINISTRA_ AL POSTO DELLA MOLTIPLICAZIONE
 *	basterebbe quindi aggiungere un certo numero di zeri al numero scritto in base 2
 *	invece che moltiplicare:
 *	ciò richiede chiaramente di avere contropasso = 2^qualcosa.
 *	ES: contropasso = 256  ==>  passo = .00390625
 *	NuovaLUT dà buoni risultati in simulazione, a orecchio identici alla c_lut,
 *	che però usa una vettori lunghi quasi 50 volte di più.
 *	
 *	                                                                     :-)
 */

//	PROTOTIPI
float NuovaLut(float);
void inizNuovaLut(float);
float perLut();

//	Look Up Table
const int NN = 39;
float xx[39] = 
	{0,13.157895,26.315789,39.473684,52.631579,65.789474,78.947368,92.105263,105.263158,118.421053,131.578947,144.736842,157.894737,171.052632,184.210526,197.368421,210.526316,223.684211,236.842105,250,263.157895,276.315789,289.473684,302.631579,315.789474,328.947368,342.105263,355.263158,368.421053,381.578947,394.736842,407.894737,421.052632,434.210526,447.368421,460.526316,473.684211,486.842105,500};
float yy[39] = {0.999995, 0.999994, 0.999993, 0.999991, 0.999987, 0.999980, 0.999965, 0.999934, 0.999865, 0.999711, 0.999375, 0.998660, 0.997236, 0.994658, 0.990537, 0.984862, 0.978191, 0.971463, 0.965518, 0.960795, 0.957325, 0.954913, 0.953297, 0.952241, 0.951562, 0.951130, 0.950856, 0.950684, 0.950576, 0.950508, 0.950465, 0.950439, 0.950422, 0.950411, 0.950405, 0.950401, 0.950398, 0.950397, 0.950396};

float contropasso, x0;

int LutCNT=0;
int LutIND = 12;
int inc = 1;
