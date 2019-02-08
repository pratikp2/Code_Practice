/** ************************************************************** Boost Setup for Visual Studios *******************************************************************

	1. Inside property Manager in project property add a new property inside "Release | x64" named "Boost_Release_x64" and open it.
	2. Inside section C/C++ in "Additional Include directories" add path up to boost in side boost_version.
	3. Inside section Linker in "Additional Library directories" add path up to "boost_veriosn/stage/x64/lib"
	4. Now outside Change Solution configuratiosn to "Release" and Solution Platforms to x64 and compile.

********************************************************************************************************************************************************************/

#include "TestClasses.h"
#include <boost/serialization/version.hpp>

class Info : public Layer3
{
private:
	const unsigned int EncoderVersion = 58;

	// Allow serialization to access non-public data members.  
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int DecoderVersion)
	{
		if (DecoderVersion == EncoderVersion)
		{
			ar & filenames;
		}
		else
		{
			std::cout << " Error Occured....! Version Mismatch " << std::endl;
			std::cout << " Serialization Version	: " << EncoderVersion << std::endl;
			std::cout << " De-Serialization Version : " << DecoderVersion << std::endl;
		}
	}

	std::vector<std::string> filenames;

public:
	Test1 * ptrTest = new Test1();

	Info();
	~Info();

	void AddFilename(const std::string& filename);
	void Print() const;
};

BOOST_CLASS_VERSION(Info, 58)

Info::Info() {}
Info::~Info() { delete  ptrTest; ptrTest = NULL; }

void Info::Print() const { std::copy(filenames.begin(), filenames.end(), std::ostream_iterator<std::string>(std::cout, "\n")); }
void Info::AddFilename(const std::string& filename) { filenames.push_back(filename); }


int main(int argc, char** argv)
{
	std::vector<Info> infs;

	Info info1, info2;
	info1.AddFilename("ThisFile.txt");
	info1.AddFilename("ThatFile.txt");
	info1.AddFilename("OtherFile.txt");

	info2.AddFilename("ABC");
	info2.AddFilename("123");
	info2.AddFilename("XYZ");

	infs.push_back(info1);
	infs.push_back(info2);

	// Save filename data contained in Info object  
	{
		std::ofstream Obj_ofstream("data.dat", std::ios::binary);
		boost::archive::binary_oarchive op_archive(Obj_ofstream);
		op_archive << infs;
	}

	// Restore from saved data and print to verify contents  
	std::vector<Info> restored_info;
	{
		std::ifstream Obj_ifstream("data.dat", std::ios::binary);
		boost::archive::binary_iarchive ip_archive(Obj_ifstream);
		ip_archive >> restored_info;
	}

	std::vector<Info>::const_iterator it = restored_info.begin();

	for (; it != restored_info.end(); ++it)
	{
	    Info info = *it;
		info.Print();
	}

	it = restored_info.begin();
	Info ObjInfo = *it;

	std::cout << std::endl << ObjInfo.Layer1::ptr << std::endl;
	std::cout << ObjInfo.Layer2::ptr << std::endl;
	std::cout << ObjInfo.Layer3::ptr << std::endl;

	std::cout << std::endl << ObjInfo.ptrTest->ptr << std::endl;
	std::cout << ObjInfo.Layer1::m_ptrTest2->ptr << std::endl;

	system("PAUSE");
	return 0;
}

// ##########################################			serialize()			################################################################

// While serializing the standard data type you can define Save() and Load() Methods, and call them.

// In order to serialize objects of user-defined types, you must define the member function serialize(). This function is called when
// the object is serialized to or restored from a byte stream. Because serialize() is used for both serializing and restoring, 
// Boost.Serialization supports the operator operator& in addition to operator<< and operator>>. With operator& there is no need to 
// distinguish between serializing and restoring within serialize().

// serialize() is automatically called any time an object is serialized or restored.It should never be called explicitly and, thus,
// should be declared as private.If it is declared as private, the class boost::serialization::access must be declared as a friend 
// to allow Boost.Serialization to access the member function.

// More Info : https://theboostcpplibraries.com/boost.serialization-archive

// ##########################################			Versioning for Archive		##########################################################

// The macro BOOST_CLASS_VERSION assigns a version number to a class. If BOOST_CLASS_VERSION is not used, the version number is 0 by default.
// The version number is stored in the archive and is part of it.While the version number specified for a particular class via the BOOST_CLASS_VERSION
// macro is used during serialization, the parameter version of serialize() is set to the value stored in the archive when restoring.If the
// new version of animal accesses an archive containing an object serialized with the old version, the member variable name_ would not be 
// restored because the old version did not have such a member variable.

// ##############################################################################################################################################

/*#include <cstdlib>
#include <fstream>
#include <iostream>
#include "json/json.h"
int main(void) {
	Json::Reader reader;
	Json::Value root;
	Json::StyledStreamWriter writer;
	std::string text = "{ \"first\": \"James\", \"last\": \"Bond\", \"nums\": [0, 0, 7] }";
	std::ofstream outFile;
	// Parse JSON and print errors if needed
	if (!reader.parse(text, root)) {
		std::cout << reader.getFormattedErrorMessages();
		exit(1);
	}
	else {
		// Read and modify the json data
		std::cout << "Size: " << root.size() << std::endl;
		std::cout << "Contains nums? " << root.isMember("nums") << std::endl;
		root["first"] = "Jimmy";
		root["middle"] = "Danger";
		// Write the output to a file
		outFile.open("output.json");
		writer.write(outFile, root);
		outFile.close();
	}
	return 0;
}*/