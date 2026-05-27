#include<iostream>

#include"Crypto.hpp"
#include"Log.hpp"
#include"Utilities.hpp"

#define STREAM_TEST

int main()
{
#ifdef ENC_TEST
	encryptionApplicationTest();
#endif
#ifdef LOG_TEST
	encryptionApplicationTest();
	#endif
#ifdef STREAM_TEST
	fileStreamApplicationTest();
#endif


	return 0;
}