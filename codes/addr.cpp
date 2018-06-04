#include <bitcoin/bitcoin.hpp>
int main()
{
    //private sectet key
    bc::ec_secret secret;
    bool success = bc::decode_base16(secret, "126f8a2647546409579d9aab4f2eb62921221b6da51bdb8c9b6aaeea24e0baf9");
    assert(success);
    return 0;
}
