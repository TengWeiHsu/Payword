#include<string>
#include<Hash.h>

class User {
	public:
		size_t last_payword;
		short HashChainLength;
	
	private:
		size_t first_payword;
		
		void ComputePayword (){
			Hashnof s;
			size_t payword;
			for (int i = 0; i < HashChainLength; i++){
				hash_init(&s);
				hash_write(&s, User::first_payword, strlen(User::first_payword));
				payword = hash_result(&s);
			}
			User::last_payword = payword;	
		}
};

void Padding (std::string &str, const size_t length, const char paddingchar = ' ') {
	if (num > std::str.size())
		std::str.insert(0, num - str.size(), paddingchar)
} 

