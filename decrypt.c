#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <stdlib.h>

	/*const char key[] = {
        0x7d,0x3d,0x74,0x23,0xdd,0x7f,0x16,0xb4,0x1b,0x90,0x94,0x2c,0x8f,0xbb,0x86,0x81,\
        0x80,0x29,0xb4,0xd3,0x34,0x79,0x4d,0xdf,0x1d,0x9e,0x1c,0xe9,0x71,0xea,0x73,0x92
};*/
#define ENCRYPT_FILE "encrypt.txt"
void decrypt_openssl(const char* key)
{
        FILE *file_p;
        uint8_t iv[12];
        uint8_t tag[16];
	printf("\n%s\n",key);

        file_p = fopen(ENCRYPT_FILE,"r");
        if(file_p == NULL){
                printf("Unable to open the file\n");
                return;
        }
        fseek(file_p, 0, SEEK_END);
        long fsize = ftell(file_p);
        fseek(file_p, 0, SEEK_SET);

        if(file_p == NULL){
                printf("File cannot be open \n");

        }
        char *encrypted_text;

        encrypted_text = malloc(fsize);

        memset(encrypted_text, 0, fsize);
	int rc = fread(encrypted_text, 1, fsize, file_p);

        fclose(file_p);

        size_t out_len = 0;
        size_t in_len = 0;

        EVP_CIPHER_CTX *ctx = NULL;

        ctx = EVP_CIPHER_CTX_new();

        rc = EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL,
                                          NULL);
        if(!rc){
                printf("Failed to init (%d)\n", rc);
        }

        memcpy(iv, encrypted_text, 12);

        rc = EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);

        if(!rc){
                printf("Failed to set key (%d)\n", rc);
        }
        in_len = fsize  - 12 - 16;
        char outbuf[in_len];

        printf("Input length = %d\n", in_len);

        memset(outbuf, 0, in_len);

        rc = EVP_DecryptUpdate(ctx, outbuf, &out_len,  &encrypted_text[12], in_len);

        if(!rc){
                printf("Failed to decrypt update (%d)\n", rc);
		}

        rc = EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG,
                                               16, &encrypted_text[in_len + 12]);
        if(!rc){
                printf("Failed to set tag (%d)\n", rc);
        }
        rc = EVP_DecryptFinal_ex(ctx, outbuf, &out_len);
        if(!rc){
                printf("Failed to decrypt final (%d)\n", rc);
        }
        printf("Decrypted text = \n %s\n", outbuf);

        FILE *op;
        op = fopen("decrypt.json", "w");
         if(op == NULL){
                printf("download failed \n");
         }
        fprintf(op,"%s",&outbuf);

        fclose(op);

}
