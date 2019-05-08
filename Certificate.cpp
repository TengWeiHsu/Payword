#include<string>

class Certificate {
	protected:
		std::string BrokerIdentity,
			   UserIdentity;
		unsigned long long BrokerPublicKey,
						   UserPublicKey;
		int ExpireDate,
			CreditLimit;
		
	public:
		Certificate () {};
		Certificate (std::string b, std::string u, unsigned long long bpk, 
					 unsigned long long upk, int ed, int cl) : 
			BrokerIdentity(b),
			UserIdentity(u)
			BrokerPublicKey(bpk),
			UserPublicKey(upk),
			ExpireDate(ed),
			CreditLimit(cl) {};
		
		bool Verify();
		void Sign();
		
		unsigned long long GetUserPublicKey() {return this->UserPublicKey;};
		string GetUserIdentity() {return this->UserIdentity;};
		
		friend class Commit;
};
