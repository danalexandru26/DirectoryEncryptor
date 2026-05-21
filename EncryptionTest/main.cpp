#include<botan/auto_rng.h>
#include<botan/cipher_mode.h>
#include<botan/block_cipher.h>
#include<botan/hex.h>
#include<botan/rng.h>

#include<iostream>

#include"Crypto.hpp"


int main() {
	Botan::AutoSeeded_RNG rng;
	const std::string plainText{ "Hello World!" };

	const Botan::secure_vector<uint8_t> key = Botan::hex_decode_locked("2B7E151628AED2A6ABF7158809CF4F3C");

	const auto enc = Botan::Cipher_Mode::create_or_throw("AES-128/CBC/PKCS7", Botan::Cipher_Dir::Encryption);
	enc->set_key(key);

	//generate IV
	const auto iv = rng.random_vec<std::vector < uint8_t >> (enc->default_nonce_length());

	Botan::secure_vector<uint8_t> pt(plainText.data(), plainText.data() + plainText.length());

	enc->start(iv);
	enc->finish(pt);

	std::cout << enc->name() << "Plain text: " << Botan::hex_encode(pt) << '\n';

	auto dec = Botan::Cipher_Mode::create_or_throw("AES-128/CBC/PKCS7", Botan::Cipher_Dir::Decryption);
	dec->set_key(key);
	dec->start(iv);
	dec->finish(pt);

	std::cout << pt.data() << '\n';


	const std::string path = "D:\\Crypto\\SampleFiles";
	const std::string cryptoKey = "2B7E151628AED2A6ABF7158809CF4F3C";

	DirectoryEncryptor fcrypt(cryptoKey, path);

	fcrypt.EncryptDirectory();


	return 0;
}