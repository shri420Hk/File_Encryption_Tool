#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define KEY_SIZE 32 // 256 bits key size

bool encryptFile(const char* inputFile, const char* outputFile, const unsigned char* key) {
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin) {
        std::cerr << "Error opening input file." << std::endl;
        return false;
    }

    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout) {
        std::cerr << "Error opening output file." << std::endl;
        fin.close();
        return false;
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating encryption context." << std::endl;
        fin.close();
        fout.close();
        return false;
    }

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, nullptr);

    const int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    unsigned char inBuf[1024], outBuf[1024 + blockSize];
    int numRead = 0, numWritten = 0;

    while ((numRead = fin.read(reinterpret_cast<char*>(inBuf), sizeof(inBuf)).gcount()) > 0) {
        EVP_EncryptUpdate(ctx, outBuf, &numWritten, inBuf, numRead);
        fout.write(reinterpret_cast<char*>(outBuf), numWritten);
    }

    int finalWritten;
    EVP_EncryptFinal_ex(ctx, outBuf, &finalWritten);
    fout.write(reinterpret_cast<char*>(outBuf), finalWritten);

    EVP_CIPHER_CTX_free(ctx);

    fin.close();
    fout.close();

    return true;
}

bool decryptFile(const char* inputFile, const char* outputFile, const unsigned char* key) {
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin) {
        std::cerr << "Error opening input file." << std::endl;
        return false;
    }

    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout) {
        std::cerr << "Error opening output file." << std::endl;
        fin.close();
        return false;
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating decryption context." << std::endl;
        fin.close();
        fout.close();
        return false;
    }

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, nullptr);

    const int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    unsigned char inBuf[1024 + blockSize], outBuf[1024];
    int numRead = 0, numWritten = 0;

    while ((numRead = fin.read(reinterpret_cast<char*>(inBuf), sizeof(inBuf)).gcount()) > 0) {
        EVP_DecryptUpdate(ctx, outBuf, &numWritten, inBuf, numRead);
        fout.write(reinterpret_cast<char*>(outBuf), numWritten);
    }

    int finalWritten;
    EVP_DecryptFinal_ex(ctx, outBuf, &finalWritten);
    fout.write(reinterpret_cast<char*>(outBuf), finalWritten);

    EVP_CIPHER_CTX_free(ctx);

    fin.close();
    fout.close();

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <encrypt/decrypt> <input_file> <output_file> <key>" << std::endl;
        return 1;
    }

    const char* mode = argv[1];
    const char* inputFile = argv[2];
    const char* outputFile = argv[3];
    const unsigned char* key = reinterpret_cast<const unsigned char*>(argv[4]);

    if (strcmp(mode, "encrypt") == 0) {
        if (strlen(reinterpret_cast<const char*>(key)) != KEY_SIZE) {
            std::cerr << "Key size must be " << KEY_SIZE << " bytes." << std::endl;
            return 1;
        }

        if (encryptFile(inputFile, outputFile, key)) {
            std::cout << "File encrypted successfully." << std::endl;
        } else {
            std::cerr << "Error encrypting file." << std::endl;
            return 1;
        }
    } else if (strcmp(mode, "decrypt") == 0) {
        if (strlen(reinterpret_cast<const char*>(key)) != KEY_SIZE) {
            std::cerr << "Key size must be " << KEY_SIZE << " bytes." << std::endl;
            return 1;
        }

        if (decryptFile(inputFile, outputFile, key)) {
            std::cout << "File decrypted successfully." << std::endl;
        } else {
            std::cerr << "Error decrypting file." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Invalid mode. Use 'encrypt' or 'decrypt'." << std::endl;
        return 1;
    }

    return 0;
}
