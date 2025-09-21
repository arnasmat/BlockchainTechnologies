#ifndef OTHER_ALGORITHMS_H
#define OTHER_ALGORITHMS_H
#include "HashGenInterface.h"
#include <iostream>
#include <openssl/sha.h>
#include <openssl/evp.h>

class Sha256: public HashGenInterface {
public:
	std::string generateHash(const std::string &input) const override {
		unsigned char hash[SHA256_DIGEST_LENGTH];
		SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

		std::stringstream ss;
		for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
		return ss.str();
	}
};

class Md5: public HashGenInterface {
public:
	std::string generateHash(const std::string &input) const override {
		EVP_MD_CTX*   context = EVP_MD_CTX_new();
		const EVP_MD* md = EVP_md5();
		unsigned char md_value[EVP_MAX_MD_SIZE];
		unsigned int  md_len;
		std::string        output;

		EVP_DigestInit_ex2(context, md, NULL);
		EVP_DigestUpdate(context, input.c_str(), input.length());
		EVP_DigestFinal_ex(context, md_value, &md_len);
		EVP_MD_CTX_free(context);

		output.resize(md_len * 2);
		for (unsigned int i = 0 ; i < md_len ; ++i)
			std::sprintf(&output[i * 2], "%02x", md_value[i]);
		return output;
	}
};
#endif // OTHER_ALGORITHMS_H
