// hw brickbench
// benchmark test for NXT
// PL: nxtOSEK TOPPERS/ATK C/C++
// Autor: (C) Helmut Wunder 2013
// Ported to nxtOSEK by Martin Aumair
// http://roboticsaumair.jimdo.com/
// freie Verwendung für private Zwecke
// für kommerzielle Zwecke nur nach Genehmigung durch den Autor.
// version 1.08alpha

// Runs only on nxtBios (RAM+ROM)

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
#include "../../../lejos_nxj/src/nxtvm/platform/nxt/mytypes.h"
}

#include "../../../NXtpandedLib/src/NLine.hpp"
#include "../../../NXtpandedLib/src/NRectangle.hpp"
#include "../../../NXtpandedLib/src/NCircle.hpp"
#include "../../../NXtpandedLib/src/NEllipse.hpp"

#include "../../../../GNUARM/arm-elf/sys-include/math.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////OSEK HOOKS/IRS///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void) { }

// hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

bool btnhit() {
   return (ecrobot_is_RUN_button_pressed() || ecrobot_is_ENTER_button_pressed());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////BENCHMARK FUNCTIONS////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int a[500], b[500], c[500];

//--------------------------------------------
// Mersenne Twister
//--------------------------------------------

unsigned long randM(void) {
   const int M = 7;
   const unsigned long A[2] = { 0, 0x8ebfd028 };

   static unsigned long y[25];
   static int index = 25 + 1;

   if (index >= 25) {
      int k;
      if (index > 25) {
         unsigned long r = 9, s = 3402;
         for (k = 0; k < 25; ++k) {
            r = 509845221 * r + 3;
            s *= s + 1;
            y[k] = s + (r >> 10);
         }
      }
      for (k = 0; k < 25 - M; ++k)
         y[k] = y[k + M] ^ (y[k] >> 1) ^ A[y[k] & 1];
      for (; k < 25; ++k)
         y[k] = y[k + (M - 25)] ^ (y[k] >> 1) ^ A[y[k] & 1];
      index = 0;
   }

   unsigned long e = y[index++];
   e ^= (e << 7) & 0x2b5b2500;
   e ^= (e << 15) & 0xdb8b0000;
   e ^= (e >> 16);
   return e;
}


void MatrixMatrixMult(int N, int M, int K, float A[][2], float B[][2], float C[][2]) {
   int i, j, s;
   for (i = 0; i < N; ++i) {
      for (j = 0; j < K; ++j) {
         C[i][j] = 0;
         for (s = 0; s < M; ++s) {
            C[i][j] = C[i][j] + A[i][s] * B[s][j];
         }
      }
   }
}



// no VLA!
float MatrixDet(int N, float A[]) {
   int i, j, i_count, j_count, count = 0;
   float Asub[N - 1][N - 1], det = 0;

   if (N == 1)
      return *A;
   if (N == 2)
      return ((*A) * (*(A+1+1*N)) - (*(A+1*N)) * (*(A+1)));

   for (count = 0; count < N; count++) {
      i_count = 0;
      for (i = 1; i < N; i++) {
         j_count = 0;
         for (j = 0; j < N; j++) {
            if (j == count)
               continue;
            Asub[i_count][j_count] = *(A+i+j*N);
            j_count++;
         }
         i_count++;
      }
      det += pow(-1, count) * A[0+count*N] * MatrixDet(N - 1, &Asub[0][0]);
   }
   return det;
}


//--------------------------------------------
// shell sort
//--------------------------------------------

void shellsort(int size, int* A)
{
  int i, j, increment;
  int temp;
  increment = size / 2;

  while (increment > 0) {
    for (i = increment; i < size; i++) {
      j = i;
      temp = A[i];
      while ((j >= increment) && (A[j-increment] > temp)) {
        A[j] = A[j - increment];
        j = j - increment;
      }
      A[j] = temp;
    }

    if (increment == 2)
       increment = 1;
    else
       increment = (unsigned int) (increment / 2.2);
  }
}


//--------------------------------------------
// benchmark test procedures
//--------------------------------------------

int test_Int_Add() {
   int i=1, j=11, k=112, l=1111, m=11111, n=-1, o=-11, p=-111, q=-1112, r=-11111;
   int x;
   volatile long s=0;
   for(x=0;x<10000;++x) {
     s+=i; s+=j; s+=k; s+=l; s+=m; s+=n; s+=o; s+=p; s+=q; s+=r;
   }
   return s;
}






long test_Int_Mult() {
  int x,y;
  volatile long s;

  for(y=0;y<2000;++y) {
    s=1;
    for(x=1;x<=13;++x) { s*=x;}
    for(x=13;x>0;--x) { s/=x;}

  }
  return s;
}



float test_float_math() {
   float s = M_PI;
   int y;

   for (y = 0; y < 5000; ++y) {
      s *= sqrt(s);
      s = sin(s);
      s *= cos(10.5 * s);
      s = sqrt(s);
      s = exp(s);
   }
   return s;
}


long test_rand_MT() {
   volatile unsigned long s;
   int y;

   for (y = 0; y < 5000; ++y) {
      s = randM() % 10001;
   }
   return s;
}

float test_matrix_math() {
   int x;

   float A[2][2], B[2][2], C[2][2];
   float O[3][3], T[3][3];
   unsigned long s;

   for (x = 0; x < 250; ++x) {

      A[0][0] = 1;
      A[0][1] = 3;
      A[1][0] = 2;
      A[1][1] = 4;

      B[0][0] = 10;
      B[0][1] = 30;
      B[1][0] = 20;
      B[1][1] = 40;

      MatrixMatrixMult(2, 2, 2, A, B, C);

      A[0][0] = 1;
      A[0][1] = 3;
      A[1][0] = 2;
      A[1][1] = 4;

      MatrixDet(2, &A[0][0]);

      O[0][0] = 1;
      O[0][1] = 4;
      O[0][2] = 7;
      O[1][0] = 2;
      O[1][1] = 5;
      O[1][2] = 8;
      O[2][0] = 3;
      O[2][1] = 6;
      O[2][2] = 9;

      MatrixDet(3, &O[0][0]);

   }

   s = (O[0][0] * O[1][1] * O[2][2]);
   return s;
}


long test_Sort(){
  unsigned long s;
  int y, i;
  int t[500];

  for(y=0;y<30;++y) {
    for(i=0; i<500;++i) {t[i]=a[i];}
    shellsort(500, t);
    for(i=0; i<500;++i) {t[i]=b[i];}
    shellsort(500, t);
    for(i=0; i<500;++i) {t[i]=c[i];}
    shellsort(500, t);
  }
  return y;
}


// helper functions for output
// normally with cout our sprintf but I think in a benchmark we take the fastest method
void NumOut(int x, int y, int num, U32 places) {
   display_goto_xy(x, y);
   display_int(num, places);
}

void TextOut(int x, int y, const char *str) {
   display_goto_xy(x, y);
   display_string(str);
}

long test_TextOut() {
   int y;

   for (y = 0; y < 20; ++y) {
      display_clear(0);

      // x,y value is line not pixel! nxtOSEK use fast dma update in chars,lines!
      NumOut(0,0, 0, 1); NumOut(2,0, 1000, 4); TextOut(8,0, "int_Add");
      NumOut(0,1, 1, 1); NumOut(2,1, 1010, 4); TextOut(8,1, "int_Mult");
      NumOut(0,2, 2, 1); NumOut(2,2, 1020, 4); TextOut(8,2, "float_op");
      NumOut(0,3, 3, 1); NumOut(2,3, 1030, 4); TextOut(8,3, "rand_array");
      NumOut(0,4, 4, 1); NumOut(2,4, 1040, 4); TextOut(8,4, "matrx_algb");
      NumOut(0,5, 5, 1); NumOut(2,5, 1050, 4); TextOut(8,5, "arr_sort");
      NumOut(0,6, 6, 1); NumOut(2,6, 1060, 4); TextOut(8,6, "displ_txt");
      NumOut(0,7, 7, 1); NumOut(2,7, 1070, 4); TextOut(8,7, "testing...");
      display_update();  // not really good because we can update hardware only every 16 ms
   }
   return 99;
}

using namespace nxpl; // NXtpandedLib
NLcd lcd;

long test_graphics() {
   int x = 0;
   for (int y = 0; y < 100; ++y) {
	  lcd.clear();
      // no official functions, its my actual project: NXtpandedLib
      NCircle::draw(NPoint(50, 40), 10);
      NCircleFilled::draw(NPoint(30, 24), 10);
      NLine::draw(NPoint(10, 10), NPoint(60, 60));
      NLine::draw(NPoint(50, 20), NPoint(90, 70));
      NRectangle::draw(NPixelBox(NPoint(20, 20), 40, 40));
      NRectangleFilled::draw(NPixelBox(NPoint(65, 25), 20, 30));
      NEllipse::draw(NPoint(70, 30), 15, 20);

      NLcd::update(); // not really good because we can update hardware only every 16 ms
   }
   return x;
}



U32 runtime[8];

void displayValues() {
   display_clear(0);

   NumOut(0,0, 0, 1); NumOut(2,0, (int)runtime[0], 5); TextOut(8,0, "int_Add");
   NumOut(0,1, 1, 1); NumOut(2,1, (int)runtime[1], 5); TextOut(8,1, "int_Mult");
   NumOut(0,2, 2, 1); NumOut(2,2, (int)runtime[2], 5); TextOut(8,2, "float_op");
   NumOut(0,3, 3, 1); NumOut(2,3, (int)runtime[3], 5); TextOut(8,3, "rand_array");
   NumOut(0,4, 4, 1); NumOut(2,4, (int)runtime[4], 5); TextOut(8,4, "matrx_algb");
   NumOut(0,5, 5, 1); NumOut(2,5, (int)runtime[5], 5); TextOut(8,5, "arr_sort");
   NumOut(0,6, 6, 1); NumOut(2,6, (int)runtime[6], 5); TextOut(8,6, "displ_txt");
   NumOut(0,7, 7, 1); NumOut(2,7, (int)runtime[7], 5); TextOut(8,7, "grafics");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////TASKS ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


TASK(TaskMain) {
   //test_graphics();
   //display_update();


   unsigned long time0, x, y;
   float s;

   display_clear(1);

   TextOut(0, 0, "hw brickbench");
   TextOut(0, 1, "(C)H.Wunder 2013");
   TextOut(0, 3, "nxtOSEK port:");
   TextOut(0, 4, "Martin Aumair");
   TextOut(0, 5, "initializing...");
   display_update();
   systick_wait_ms(3000);

   for (y = 0; y < 500; ++y) {
      a[y] = randM();
      b[y] = randM();
      c[y] = randM();
   }

   TextOut(0, 6, "done .. go!");
   display_update();
   systick_wait_ms(2000);
   display_clear(1);

   time0 = systick_get_ms();
   s = test_Int_Add();
   runtime[0] = systick_get_ms() - time0;
   NumOut(0,0, 0, 1); NumOut(2,0, (int)runtime[0], 5); TextOut(8,0, "int_Add");
   display_update();

   time0 = systick_get_ms();
   s = test_Int_Mult();
   runtime[1] = systick_get_ms() - time0;
   NumOut(0,1, 1, 1); NumOut(2,1, (int)runtime[1], 5); TextOut(8,1, "int_Mult");
   display_update();

   time0 = systick_get_ms();
   s = test_float_math();
   runtime[2] = systick_get_ms() - time0;
   NumOut(0,2, 2, 1); NumOut(2,2, (int)runtime[2], 5); TextOut(8,2, "float_op");
   display_update();

   time0 = systick_get_ms();
   s = test_rand_MT();
   runtime[3] = systick_get_ms() - time0;
   NumOut(0,3, 3, 1); NumOut(2,3, (int)runtime[3], 5); TextOut(8,3, "rand_array");
   display_update();

   systick_wait_ms(1500);

   time0 = systick_get_ms();
   s = test_matrix_math();
   runtime[4] = systick_get_ms() - time0;
   NumOut(0,4, 4, 1); NumOut(2,4, (int)runtime[4], 5); TextOut(8,4, "matrx_algb");
   display_update();

   systick_wait_ms(1500);

   time0 = systick_get_ms();
   s = test_Sort();
   runtime[5] = systick_get_ms() - time0;
   NumOut(0,5, 5, 1); NumOut(2,5, (int)runtime[5], 5); TextOut(8,5, "arr_sort");
   display_update();

   time0 = systick_get_ms();
   s = test_TextOut();
   runtime[6] = systick_get_ms() - time0;
   display_update();


   time0 = systick_get_ms();
   s = test_graphics();
   runtime[7] = systick_get_ms() - time0;

   displayValues();

   while (! btnhit() );  //  <<<<< wait for btnpress

   display_clear(1);
   y = 0;
   for (x = 0; x < 8; ++x) {
      y += runtime[x];
   }

   TextOut(0,0, "sum ms:");    NumOut(8,0, y, 7);
   TextOut(0,1, "benchmark:"); NumOut(8,1, 50000000/y, 7);

   systick_wait_ms(1000);
   while (! btnhit() );  //  <<<<< wait for btnpress

   TerminateTask();
}

}
