#include "kod.h"
int main (const int argc, const char ** argv) {
    // max komut karakter sayısı
    char komut[1000];

    while (1) {


        printf(">");
        satirOku(komut);

        while (komut[0] == '\0') {
            printf(">  ");
             satirOku(komut);
        }

        // quit veya exitin ardından 3 tane daha karakter alsada sonlanıyor
        if (strncmp (komut, "quit" , 4) == 0)
            return 0;

        if (strncmp (komut, "exit" , 4) == 0)
            return 0;
        
        komutBolCalistir(komut);
    }
    return 0;
}
