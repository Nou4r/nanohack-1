#pragma once
#include <CryptoPP/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/ccm.h>

#include <atlsecurity.h> 
#include <strsafe.h> 
#include <windows.h>
#include <string>
#include <stdio.h>
#include <iostream>

#include <curl/curl.h>

#include <shellapi.h>
#include <wininet.h>

#include <sstream> 
#include <iomanip> 
#include <fstream> 

#include <nlohmann/json.hpp>

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Urlmon.lib")

#define PINNEDPUBLICKEY "sha256//IiRZsYyu+HwIESNlvssbuLrPJjctshjK3ktg+JsQXnU="
#define API_URL "https://auth.plusminus.vip/api/1.0/"
#define NO_PROXY_URL "auth.plusminus.vip"

#define SSLPINNING

std::string new_name = xorstr_("");

namespace Authentication {

	class encryption {
	public:
		std::string name;
		static std::string encrypt_string(const std::string& plain_text, const std::string& key, const std::string& iv) {
			std::string cipher_text;
			VM_EAGLE_BLACK_START
				try {
				CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
				encryption.SetKeyWithIV((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());

				CryptoPP::StringSource encryptor(plain_text, true,
					new CryptoPP::StreamTransformationFilter(encryption,
						new CryptoPP::HexEncoder(
							new CryptoPP::StringSink(cipher_text),
							false
						)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				exit(0);
			}

			VM_EAGLE_BLACK_END

				return cipher_text;
		}

		static std::string decrypt_string(const std::string& cipher_text, const std::string& key, const std::string& iv) {
			std::string plain_text;
			VM_EAGLE_BLACK_START
				try {
				CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
				decryption.SetKeyWithIV((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());

				CryptoPP::StringSource decryptor(cipher_text, true,
					new CryptoPP::HexDecoder(
						new CryptoPP::StreamTransformationFilter(decryption,
							new CryptoPP::StringSink(plain_text)
						)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				exit(0);
			}
			VM_EAGLE_BLACK_END
				return plain_text;
		}

		static std::string sha256(const std::string& plain_text) {
			std::string hashed_text;
			CryptoPP::SHA256 hash;
			VM_EAGLE_BLACK_START
				try {
				CryptoPP::StringSource hashing(plain_text, true,
					new CryptoPP::HashFilter(hash,
						new CryptoPP::HexEncoder(
							new CryptoPP::StringSink(hashed_text),
							false
						)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				exit(0);
			}

			VM_EAGLE_BLACK_END

				return hashed_text;
		}

		static std::string encode(const std::string& plain_text) {
			std::string encoded_text;
			VM_EAGLE_BLACK_START
				try {
				CryptoPP::StringSource encoding(plain_text, true,
					new CryptoPP::HexEncoder(
						new CryptoPP::StringSink(encoded_text),
						false
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				exit(0);
			}
			VM_EAGLE_BLACK_END
				return encoded_text;
		}

		static std::string decode(const std::string& encoded_text) {
			std::string out;
			VM_EAGLE_BLACK_START
				try {
				CryptoPP::StringSource decoding(encoded_text, true,
					new CryptoPP::HexDecoder(
						new CryptoPP::StringSink(out)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				exit(0);
			}
			VM_EAGLE_BLACK_END
				return out;
		}

		static std::string iv_key() {
			VM_EAGLE_BLACK_START
				UUID uuid = { 0 };
			std::string guid;

			::UuidCreate(&uuid);

			RPC_CSTR szUuid = NULL;
			if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
			{
				guid = (char*)szUuid;
				::RpcStringFreeA(&szUuid);
			}

			VM_EAGLE_BLACK_END

				return guid.substr(0, 16);
		}

		static std::string encrypt(std::string message, std::string enc_key, std::string iv) {
			VM_EAGLE_BLACK_START
				enc_key = sha256(enc_key).substr(0, 32);
			iv = sha256(iv).substr(0, 16);
			VM_EAGLE_BLACK_END
				return encrypt_string(message, enc_key, iv);
		}

		static std::string decrypt(std::string message, std::string enc_key, std::string iv) {
			VM_EAGLE_BLACK_START
				enc_key = sha256(enc_key).substr(0, 32);

			iv = sha256(iv).substr(0, 16);
			VM_EAGLE_BLACK_END
				return decrypt_string(message, enc_key, iv);
		}
	};

	class utils {
	public:

		static std::string get_hwid() {
			return SystemFingerprint::CreateUniqueFingerprint()->ToString();
		}

		static std::time_t string_to_timet(std::string timestamp) {
			VM_EAGLE_BLACK_START
				auto cv = strtol(timestamp.c_str(), NULL, 10);
			VM_EAGLE_BLACK_END
				return (time_t)cv;
		}

		static std::tm timet_to_tm(time_t timestamp) {
			VM_EAGLE_BLACK_START
				std::tm context;

			localtime_s(&context, &timestamp);
			VM_EAGLE_BLACK_END
				return context;
		}

	};


	auto iv = encryption::sha256(encryption::iv_key());
	class api {
	public:

		std::string name, ownerid, secret, version;
		int days_left = 0;

		api(std::string name, std::string ownerid, std::string secret, std::string version)
			: name(name), ownerid(ownerid), secret(secret), version(version) {}

		void init()
		{
			VM_EAGLE_BLACK_START

			enckey = encryption::sha256(encryption::iv_key());
			if (ownerid.length() != 10 || secret.length() != 64)
			{
				exit(0);
			}

			auto data =
				xorstr_("type=") + encryption::encode(xorstr_("init")) +
				xorstr_("&ver=") + encryption::encrypt(version, secret, iv) +
				xorstr_("&enckey=") + encryption::encrypt(enckey, secret, iv) +
				xorstr_("&name=") + encryption::encode(name) +
				xorstr_("&ownerid=") + encryption::encode(ownerid) +
				xorstr_("&init_iv=") + iv;

			auto response = req(data);

			response = encryption::decrypt(response, secret, iv);
			auto json = response_decoder.parse(response);

			if (json[xorstr_("success")])
			{
				sessionid = json[xorstr_("sessionid")];
				// optional success message
			}
			else if (json[xorstr_("message")] == xorstr_("invalidver")) {}
			else
			{
				exit(0);
			}

			VM_EAGLE_BLACK_END
		}

		void login(std::string username, std::string password)
		{
			VM_EAGLE_BLACK_START
				std::string hwid = utils::get_hwid();
			auto iv = encryption::sha256(encryption::iv_key());
			auto data =
				xorstr_("type=") + encryption::encode(xorstr_("login")) +
				xorstr_("&username=") + encryption::encrypt(username, enckey, iv) +
				xorstr_("&pass=") + encryption::encrypt(password, enckey, iv) +
				xorstr_("&hwid=") + encryption::encrypt(hwid, enckey, iv) +
				xorstr_("&sessionid=") + encryption::encode(sessionid) +
				xorstr_("&name=") + encryption::encode(name) +
				xorstr_("&ownerid=") + encryption::encode(ownerid) +
				xorstr_("&init_iv=") + iv;
			auto response = req(data);
			response = encryption::decrypt(response, enckey, iv);
			auto json = response_decoder.parse(response);

			if (json[xorstr_("success")])
			{
				load_user_data(json[xorstr_("info")]);
				std::time_t t = std::time(0);
				std::tm now;
				localtime_s(&now, &t);
				std::tm expiry = user_data.expiry;

				this->days_left = std::difftime(mktime(&expiry), mktime(&now)) / (60 * 60 * 24);
			}
			else
			{
				exit(0);
			}

			VM_EAGLE_BLACK_END
		}

		std::string var(std::string varid) {
			VM_EAGLE_BLACK_START
				auto iv = encryption::sha256(encryption::iv_key());
			auto data =
				xorstr_("type=") + encryption::encode(xorstr_("var")) +
				xorstr_("&varid=") + encryption::encrypt(varid, enckey, iv) +
				xorstr_("&sessionid=") + encryption::encode(sessionid) +
				xorstr_("&name=") + encryption::encode(name) +
				xorstr_("&ownerid=") + encryption::encode(ownerid) +
				xorstr_("&init_iv=") + iv;
			auto response = req(data);
			response = encryption::decrypt(response, enckey, iv);
			auto json = response_decoder.parse(response);

			if (json[xorstr_("success")])
			{
				return json[xorstr_("message")];
			}
			VM_EAGLE_BLACK_END
		}

		void log(std::string message) {
			VM_EAGLE_BLACK_START

				auto iv = encryption::sha256(encryption::iv_key());

			char acUserName[100];
			DWORD nUserName = sizeof(acUserName);
			GetUserNameA(acUserName, &nUserName);
			std::string UsernamePC = acUserName;

			std::string data =
				xorstr_("type=") + encryption::encode(xorstr_("log")) +
				xorstr_("&pcuser=") + encryption::encrypt(UsernamePC, enckey, iv) +
				xorstr_("&message=") + encryption::encrypt(message, enckey, iv) +
				xorstr_("&sessionid=") + encryption::encode(sessionid) +
				xorstr_("&name=") + encryption::encode(name) +
				xorstr_("&ownerid=") + encryption::encode(ownerid) +
				xorstr_("&init_iv=") + iv;

			req(data);
			VM_EAGLE_BLACK_END
		}

		void webhook(std::string id, std::string params) {

			VM_EAGLE_BLACK_START
				auto iv = encryption::sha256(encryption::iv_key());
			std::string hwid = utils::get_hwid();

			std::string data =
				xorstr_("type=") + encryption::encode(xorstr_("webhook")) +
				xorstr_("&webid=") + encryption::encrypt(id, secret, iv) +
				xorstr_("&params=") + encryption::encrypt(params, secret, iv) +
				xorstr_("&sessionid=") + encryption::encode(sessionid) +
				xorstr_("&name=") + encryption::encode(name) +
				xorstr_("&ownerid=") + encryption::encode(ownerid) +
				xorstr_("&init_iv=") + iv;

			auto response = req(data);
			response = encryption::decrypt(response, enckey, iv);
			auto json = response_decoder.parse(response);

			if (json[xorstr_("success")])
			{
				// optional success message
			}
			VM_EAGLE_BLACK_END
		}

		class user_data_class {
		public:
			std::string username;
			std::tm expiry;
			std::string subscription;
		};

		user_data_class user_data;

	private:
		std::string sessionid, enckey;

		static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
			VM_EAGLE_BLACK_START
			((std::string*)userp)->append((char*)contents, size * nmemb);
			VM_EAGLE_BLACK_END
				return size * nmemb;
		}

		static std::string req(std::string data) {
			CURL* curl = curl_easy_init();

			if (!curl)
				return xorstr_("null");

			std::string to_return;

			curl_easy_setopt(curl, CURLOPT_URL, xorstr_(API_URL));
			curl_easy_setopt(curl, CURLOPT_USERAGENT, xorstr_("KeyAuth"));

			curl_easy_setopt(curl, CURLOPT_NOPROXY, xorstr_(NO_PROXY_URL));

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

#ifdef SSLPINNING
			curl_easy_setopt(curl, CURLOPT_PINNEDPUBLICKEY, xorstr_(PINNEDPUBLICKEY));
#endif
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);

			auto code = curl_easy_perform(curl);

			//if (code != CURLE_OK)
			//	MessageBoxA(0, curl_easy_strerror(code), 0, MB_ICONERROR);

			return to_return;
		}

		class user_data_structure {
		public:
			std::string username;
			std::string expiry;
			std::string subscription;
		};

		void load_user_data(nlohmann::json data) {
			user_data.username = data[xorstr_("username")];
			user_data.expiry = utils::timet_to_tm(
				utils::string_to_timet(data[xorstr_("subscriptions")][0][xorstr_("expiry")])
			);
			user_data.subscription = data[xorstr_("subscriptions")][0][xorstr_("subscription")];
		}

		nlohmann::json response_decoder;

	};
}
