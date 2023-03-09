//
// Copyright(c) 2023 Magnus Grander.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#include <iostream>
#include <sstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define TINYDDSLOADER_IMPLEMENTATION
#include "tinyddsloader.h"

using namespace std;
using namespace tinyddsloader;

string repext(string filename) {
	size_t pos = filename.rfind('.');
	if(pos != string::npos) {
     filename.replace(pos, filename.length() - pos, ".png");
     return filename;
	}
	else
	{
     filename.append(".png");
     return filename;
	}	
}

string remove_extension(string filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

string IntToString(int a)
{
    ostringstream temp;
    temp << a;
    return temp.str();
}
  
int main(int argc, char** argv)
{	
	if(argc==1)
        printf("\nToo few arguments. Needs a filename to convert.\n");
    if(argc>=2)
    {    
		DDSFile dds;		
		auto ret = dds.Load(argv[1]);
		if (tinyddsloader::Result::Success != ret) {
			std::cout << "Failed to load.[" << argv[1] << "]\n";
			std::cout << "Error code : " << int(ret) << "\n";
			    switch(ret)
				{ 
					case 1:
					    cout <<"Could not open file.\n";
					case 2:
						cout <<"Could not read the file.\n";
					break;
					case 3: 
						cout <<"Error magic word found\n";
					break;
					case 4: 
						cout <<"Error wrong file size\n";
						break;
					case 5: 
						cout << "Error could not be verified.\n";
						break;
					case 6: 
						cout << "Error not supported file type\n";
						break;
					case 7: 
						cout << "Error invalid data\n";
						break;
					default:
						cout << "That Error isn’t valid\n";
					}			
		return 1;
		}
		
		std::cout << "Width: " << dds.GetWidth() << "\n";
		std::cout << "Height: " << dds.GetHeight() << "\n";
		std::cout << "Depth: " << dds.GetDepth() << "\n";
		
		int w = dds.GetWidth();
		int h = dds.GetHeight();
		int c = dds.GetDepth();
		string fn = argv[1];
		string nfn = "";
		int counter = 0;

		std::cout << "Mip: " << dds.GetMipCount() << "\n";
		std::cout << "Array: " << dds.GetArraySize() << "\n";
		
		for (uint32_t arrayIdx = 0; arrayIdx < dds.GetArraySize(); arrayIdx++) {
			for (uint32_t mipIdx = 0; mipIdx < dds.GetMipCount(); mipIdx++) {
				const auto* imageData = dds.GetImageData(mipIdx, arrayIdx);
															
				if(counter>0) {	
					nfn = "";	
					fn = argv[1];								
					nfn = remove_extension(fn);					
					fn = nfn + IntToString(arrayIdx) + IntToString(mipIdx);					
				}				
				
				std::cout << " Array[" << arrayIdx << "] "
						  << "Mip[" << mipIdx << "]: "
						  << "(" << imageData->m_width << ", "
						  << imageData->m_height << ", " << imageData->m_depth
						  << ")\n";						  
						  void* data = imageData-> m_mem;					  
						  
				nfn = repext(fn);
				cout << nfn;
									
				stbi_write_png(nfn.c_str(), w, h, c, data, w * c);
				nfn = "";
						  
			counter++;
			}
		}	   
   }
    
return 0;    
}
