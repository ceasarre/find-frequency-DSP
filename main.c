#include <dsplib.h>

//zdefiniowanie ilosci próbek, czestotliwosci sygnalu oraz czetotliwosci próbkowania
#define NUM_SAMPLES 5000
#define freq 100
#define sr 48000

//deklaracja zmiennych globalnych
//krok do kumulatora
unsigned int step = 0;



//paramery filtra
//d³ugosc filtra
#define N 55
//wspó³czynniki
const int filtr_fir [] ={ -27,  -25,  -25,  -24,  -21,  -15,   -5,   13,   39,   75,  123,  184,  257,
						343,  442,  551,  669,  794,  923, 1051, 1176, 1294, 1402, 1495, 1570, 1626,
						1661, 1672, 1661, 1626, 1570, 1495, 1402, 1294, 1176, 1051,  923,  794,  669,
						551,  442,  343, 257,  184,  123,   75,   39,   13,   -5,  -15,  -21,  -24,
						-25,  -25,  -27 };

//tablice probek
int samples[NUM_SAMPLES];
int output[NUM_SAMPLES];

//bufor do filtracji z DSPLIB
int buffor[N+2];

//filtracja próbek
void blockfir(int *input, const int *filter, int *output, const int numSamples, int numFilter){

	int i,j,k = 0;
	long temp = 0;
	for (i = 0; i < numSamples; i++){

		temp = 0;
		for(j = 0 ; j < numFilter; j++){

//			zmienna pomocnicza
			k = i - j;

//			kontrola przekroczenia zakresu tablicy z lewej strony
			if((k) < 0)
				break;

			temp = _smaci(temp,*(filter + j),*(input + (k)));

		}
		temp = temp + (1<<14);

		*(output + i) = (int)(temp >> 15);

	}
}

//funkcja generuj¹ca przebieg pi³okszta³tny
void saw (int *p, unsigned int n, int step){
	static int amplitude = 0;
	int i = 0;

	for(i = 0; i < n; i++){
		*p = amplitude;
		amplitude += step;
		p++;
	}
}

//	Generowanei szumu bia³ego za pomoc¹ DSP
void noise(int *buffer, unsigned int buflen){
	rand16init();
	rand16((DATA*)buffer,buflen);
}



void main(void) {

//	Obliczanie kroku dla akumulatora
	step = (int)((22368L * freq) >> 14);


//	sygna³ pi³oszta³tny
//	saw(samples, NUM_SAMPLES, step);

//	szum bia³y
	noise(samples,NUM_SAMPLES);


//wywo³anie funkcji filtruj¹cej
	blockfir(samples, filtr_fir, output,NUM_SAMPLES,N);

// DSPLIB filtracja
//	fir((DATA*)samples, (DATA*)filtr_fir, (DATA*)output, (DATA*)buffor, NUM_SAMPLES, N);

//	filtracja próbka po próbce
//	int i;
//	for(i = 0; i < NUM_SAMPLES; i++){
//		fir((DATA*)(samples + i), (DATA*)filtr_fir, (DATA*)(output + i), (DATA*)buffor, 1, N);
//	}

	while (1); // do not exit
}
