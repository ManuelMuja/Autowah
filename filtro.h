/* FILTRO.H

Questo documento contiene le variabili necessarie al filtraggio

*/


//	PREPARAFILTRO
//	Costanti
	const int FF=3;			// lunghezza del filtro iir: numero di campioni necessarii al calcolo

//	Coefficienti
	float A, AA, A2, mAA,	// Passabanda

		b1, b0, a1, a0,	// Passabasso
		
		c, A2c;

//	Vettori
	float ing[3],	// buffer d'ingresso e uscita
		y[3],				// per avere il controllo sul volume
		rad[2], inv[2];

	int vec, nuo;

	float somma, prod;

	int dip_buf[3];
	int dip_buf_len, dip_cnt, tutti1, tutti0;
	int acceso, tmp;

void inizVett();
void preparaFiltro(int freq, float RC);
int tastino(int dip_num);
