

#include "blake3.h"

B tv[64]=
{ 0x31, 0xe9, 0x9e, 0xe6, 0x4d, 0xae, 0x52, 0x26, 
  0x37, 0x09, 0x53, 0xeb, 0x1e, 0x0b, 0x18, 0xa5, 
  0x37, 0xf5, 0x73, 0x0e, 0x57, 0x90, 0xcb, 0xf5, 
  0x05, 0x82, 0xb8, 0x75, 0xd7, 0x3b, 0x80, 0xa2, 
  0xff, 0x3c, 0x6d, 0xab, 0x29, 0x72, 0x40, 0xeb, 
  0x68, 0xe9, 0xae, 0xef, 0x3a, 0x5c, 0x17, 0x1f, 
  0x0a, 0x8e, 0xec, 0xfa, 0xaf, 0xfc, 0x3e, 0x59, 
  0x06, 0x0a, 0x01, 0x10, 0x56, 0xcf, 0xdf, 0xc8};
  
W v_idx[8]=
{ 0xC840, 0xD951, 0xEA62, 0xFB73,
  0xFA50, 0xCB61, 0xD872, 0xE943 };

Q sigma[7]=
{ 0xfedcba9876543210, 0x8fe95cb1d407a362,
  0x18fb0956e72dca43, 0x61852b04fd3e9c7a,
  0x461035278eafb9dc, 0x7462a03d1fc85be9,
  0xd743c2ae689105fb };
        
void blake3_permute(uint32_t *s, uint32_t *m) {
    W i, j, a, b, c, d, r, t;
    Q z, *p=sigma;
    
    for(i=0;i<56;) {
      z = *p++;
      
      do {
        d = v_idx[i++ % 8];
        a = (d & 15); b=(d >> 4 & 15);
        c = (d >> 8 & 15);d >>= 12;
        r = 0x07080C10;
        
        for(j=0;j<4;j++) {
          if(!(j & 1)) {
            s[a] += m[z & 15];
            z >>= 4;
          }
          s[a] += s[b];
          s[d] = R(s[d] ^ s[a], (r & 255));
          X(a, c), X(b, d);
          r >>= 8;
        }
      } while(z != 0);
    }
}