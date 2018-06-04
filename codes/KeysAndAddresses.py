import bitcoin

#创建一个随机的私钥
valid_private_key = False
while not valid_private_key:
	private_key = bitcoin.random_key()
	decoded_private_key = bitcoin.decode_privkey(private_key, 'hex')
	valid_private_key = 0 < decoded_private_key < bitcoin.N

print('Private Key (hex) is: ' + private_key)
print('Private Key (decimal) is:', decoded_private_key)

#转换私钥到WIF格式

wif_encoded_private_key = bitcoin.encode_privkey(decoded_private_key, 'wif')
print('Private Key (WIF) is: ' + wif_encoded_private_key)

#加入后缀 01 表明一个压缩的私钥
compressed_private_key = private_key + '01'
print('Private key Compressed (hex) is:' + compressed_private_key)

#通过压缩私钥生成一个WIF格式的私钥( WIF-compressed)
wif_compressed_private_key = bitcoin.encode_privkey(bitcoin.decode_privkey(compressed_private_key, 'hex'), 'wif')
print('Private Key (WIF-Compressed) is:' + wif_compressed_private_key)

#通过EC(elliptic curve)生成G点, 跟私钥相乘得到公钥的点
public_key = bitcoin.fast_multiply(bitcoin.G, decoded_private_key)
print('Public Key (x, y) corrdinates is:', public_key)

#对公钥进行编码(hex) 前缀04
hex_encoded_public_key = bitcoin.encode_pubkey(public_key, 'hex')
print('Public Key (hex) is:', hex_encoded_public_key)

#压缩公钥, 通过y是奇数或者偶数添加适应的前缀 
(public_key_x, public_key_y) = public_key
if public_key_y % 2 == 0:
	compressed_prefix = '02'
else:
	compressed_prefix = '03'
hex_compressed_public_key = compressed_prefix + bitcoin.encode(public_key_x, 16)
print('Compressed Public Key (hex) is:', hex_compressed_public_key)

#通过公钥生成一个比特币地址
bitcoin_address = bitcoin.pubkey_to_address(public_key)
print('Bitcoin Address (b58check) is:', bitcoin_address)

#通过压缩的公钥生成压缩的地址
compressed_bitcoin_address = bitcoin.pubkey_to_address(hex_compressed_public_key)
print('Compressed Bitcoin Address (b58check) is:', compressed_bitcoin_address)
