#include <bitcoin/bitcoin.hpp>

//我们要搜索的字符串
const std::string search = "1kid";

//生成一个随机的sectet key, 一个随机的 32 bytes
bc::ec_secret random_secret(std::default_random_engine& engine);

//从一个EC secret去除比特币地址
std::string bitcoin_address(const bc::ec_secret& secret);

//不区分大小写的比较
bool match_found(const std::string& address);

int main() 
{
	std::random_device random;
	std::default_random_engine engine(random());

	while(true)
	{
		//创建一个secret key
		bc::ec_secret secret = random_secret(engine);
		

		//取得地址
		std::string address = bitcoin_address(secret);
		// std::cout << "the address" << address << std::endl;

		if (match_found(address))
		{
			//success!
			std::cout << "found vanity address!" << address << std::endl;
			std::cout << "secret key" << bc::encode_base16(secret) << std::endl;
			return 0;
		}
	}

	return 0;
}

bc::ec_secret random_secret(std::default_random_engine& engine)
{
	//创建一个新的key
	bc::ec_secret secret;
	//遍历每一个字节设置一个随机的值
	for(uint8_t& byte: secret)
		byte = engine() % std::numeric_limits<uint8_t>::max();
	
	return secret;
}

std::string bitcoin_address(const bc::ec_secret& secret)
{
	//转换key到公钥
	bc::ec_compressed pubkey;
	libbitcoin::secret_to_public(pubkey, secret);

	// Pubkeyhash: sha256 + hash160
	auto my_pubkeyhash = libbitcoin::bitcoin_short_hash(pubkey);

	//创建一个地址
	// Prefix for mainnet = 0x00
	libbitcoin::one_byte addr_prefix = { { 0x00 } }; //Testnet 0x6f
	// Byte sequence = prefix + pubkey + checksum(4-bytes)
	libbitcoin::data_chunk prefix_pubkey_checksum(libbitcoin::to_chunk(addr_prefix));
	libbitcoin::extend_data(prefix_pubkey_checksum, my_pubkeyhash);
	libbitcoin::append_checksum(prefix_pubkey_checksum);
	return libbitcoin::encode_base58(prefix_pubkey_checksum);
}

bool match_found(const std::string& address)
{
	auto addr_it = address.begin();
	//比较小写字母
	for(auto it = search.begin(); it != search.end(); ++it, ++addr_it)
		if(*it != std::tolower(*addr_it))
			return false;

	return true;
}