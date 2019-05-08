#include<Hash.h>


class Broker {
	public:
		static redeem (commit, UserIdentity, VendorIdentity){
			int amount = 0, TotalPrice = 0;
			
			Total += (Commit->GetHashChainLength() - 1) * Commit->GetPrice();
			
			if (! Commit->VerifyUserCommitSignature())
				return;
		
			payword = User::last_payword;
			
			for (int i = 0; i < User::HashChainLength; i++){
				hash_init(&s);
				hash_write(&s, payword, strlen(payword));
				payword = hash_result(&s);
			}
			
			if (payword == Commit->GetFirstPayword())
				amount += Commit->GetPrice() * User::HashChainLength;
			
		}
};
