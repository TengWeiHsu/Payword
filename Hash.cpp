#include<string>
#include<cstdint>

#define BlockLength 64
#define HashLength 20

union _buffer {
	unit8_t b[BlockLength];
	unit32_t w[BlockLength / 4];
};

union _state {
	unit8_t b[HashLength];
	unit32_t w[HashLength / 4];
};

typedef struct Hashnfo {
	union _buffer buffer;
	unit8_t bufferOffset;
	union _state state;
	unit32_t ByteCount;
	unit8_t KeyBuffer[BlockLength];
	unit8_t InnerHash[HashLength];
}Hashnfo;

#define K0 0x5a827999
#define K20 0x6ed9eba1
#define K40 0x8f1bbcdc
#define K60 0xca62c1d6
 
const uint8_t sha1InitState[] = {
  0x01,0x23,0x45,0x67, // H0
  0x89,0xab,0xcd,0xef, // H1
  0xfe,0xdc,0xba,0x98, // H2
  0x76,0x54,0x32,0x10, // H3
  0xf0,0xe1,0xd2,0xc3  // H4
};

void hash_init(sha1nfo *s) {
  memcpy(s->state.b,sha1InitState,HashLength);
  s->byteCount = 0;
  s->bufferOffset = 0;
}
 
uint32_t hash_rol32(uint32_t number, uint8_t bits) {
  return ((number << bits) | (number >> (32-bits)));
}
 
void HashBlock(Hashnfo *s) {
  uint8_t i;
  uint32_t a,b,c,d,e,t;
 
  a=s->state.w[0];
  b=s->state.w[1];
  c=s->state.w[2];
  d=s->state.w[3];
  e=s->state.w[4];
  for (i=0; i<80; i++) {
    if (i>=16) {
      t = s->buffer.w[(i+13)&15] ^ s->buffer.w[(i+8)&15] ^ s->buffer.w[(i+2)&15] ^ s->buffer.w[i&15];
      s->buffer.w[i&15] = hash_rol32(t,1);
    }
    if (i<20) {
      t = (d ^ (b & (c ^ d))) + K0;
    } else if (i<40) {
      t = (b ^ c ^ d) + K20;
    } else if (i<60) {
      t = ((b & c) | (d & (b | c))) + K40;
    } else {
      t = (b ^ c ^ d) + K60;
    }
    t += hash_rol32(a,5) + e + s->buffer.w[i&15];
    e = d;
    d = c;
    c = hash_rol32(b,30);
    b = a;
    a = t;
  }
  s->state.w[0] += a;
  s->state.w[1] += b;
  s->state.w[2] += c;
  s->state.w[3] += d;
  s->state.w[4] += e;
}

void hash_addUncounted(sha1nfo *s, uint8_t data) {
  s->buffer.b[s->bufferOffset ^ 3] = data;
  s->bufferOffset++;
  if (s->bufferOffset == BlockLength) {
    HashBlock(s);
    s->bufferOffset = 0;
  }
}

void hash_writebyte(Hashnfo *s, uint8_t data) {
  ++s->ByteCount;
  hash_addUncounted(s, data);
}

void hash_write(Hashnfo *s, const char *data, size_t len) {
    for (;len--;) hash_writebyte(s, (uint8_t) *data++);
}

void hash_pad(Hashnfo *s) {
 
  // Pad with 0x80 followed by 0x00 until the end of the block
  hash_addUncounted(s, 0x80);
  while (s->bufferOffset != 56) hash_addUncounted(s, 0x00);
 
  // Append length in the last 8 bytes
  hash_addUncounted(s, 0); // We're only using 32 bit lengths
  hash_addUncounted(s, 0); // But SHA-1 supports 64 bit lengths
  hash_addUncounted(s, 0); // So zero pad the top bits
  hash_addUncounted(s, s->byteCount >> 29); // Shifting to multiply by 8
  hash_addUncounted(s, s->byteCount >> 21); // as SHA-1 supports bitstreams as well as
  hash_addUncounted(s, s->byteCount >> 13); // byte.
  hash_addUncounted(s, s->byteCount >> 5);
  hash_addUncounted(s, s->byteCount << 3);
}

uint8_t* hash_result(Hashnfo *s) {
  int i;
  // Pad to complete the last block
  hash_pad(s);
 
  // Swap byte order back
  for (i=0; i<5; i++) {
    uint32_t a,b;
    a=s->state.w[i];
    b=a<<24;
    b|=(a<<8) & 0x00ff0000;
    b|=(a>>8) & 0x0000ff00;
    b|=a>>24;
    s->state.w[i]=b;
  }
 
  // Return pointer to hash (20 characters)
  return s->state.b;
}
