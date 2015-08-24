/* * * * * * * * * * * * * * * * * * * * * */
   /* * * * * *               * * * * * */
/* * * * * * * C O N T R O L L I * * * * * */
 /* * * * * * *                 * * * * * */
  /* * * * * * * * * * * * * * * * * * * */

/*

 CCCC  OOO  N   N TTTTT RRRR   OOO  L     L     I
C     O   O NN  N   T   R   R O   O L     L     I
C     O   O N N N   T   RRR   O   O L     L     I
C     0   0 N  NN   T   R  R  O   O L     L     I
 CCCC  OOO  N   N   T   R   R  OOO  LLLLL LLLLL I

*/


float

      RC = 0.3  ,	// envelope shape

GUADAGNO = 0.15 ,	// input gain

  VOLUME = 2.0  ,	// output volume

   DISTO = 10000;	// distorsion gain





int					// use switches to enable

	wah = 0,		// autowah

	dis = 0,		// distorsion

	pha = 0;		// periodic wah








float corr;			// volume correction, automatically updated
