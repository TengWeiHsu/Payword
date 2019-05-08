#include<string>

class Commit {
	protected:
		std::string VendorIdentity;
		size_t FirstPayword;
		short price;
		
		bool VerifyBrokerCertificate();
		bool VerifyUserCommitSignture();
		
	public:
		bool Verify();
		long GetFirstPayword() {return this->FirstPayword;};
		short GetPrice() {return this->price;};
	
		
};

bool Commit::Verify(){
	if (! this->VerifyBrokerCertificate())
		return false;
	if (! this->VerifyUserCommitSignture())
		return false;
	return true;
}

bool Commit::VerifyBrokerCertificateSignature(){
	return Certificate::Verify();
}

bool Commit::VerifyUserCommitSignature (){
	bool ok = RSAVerify(signature, GetUserPublicKey());
	return ok == true;
}
