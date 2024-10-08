void activeWait(const int a){
  for(int i=0;i<a;i++){
    for(int j=0;j<a;j++){
      --j;++j;
    }
  }
}

int main(int argc, const char * argv[]){
  for(int i=10;i<(0xffffffff^0x10000000);i++){
    activeWait(0xffffffff^0x10000000);
  }
}
