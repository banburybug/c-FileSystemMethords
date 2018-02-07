
#include<Windows.h>
#include <tchar.h> 
#include<string>

std::string FromUnicodeString(WCHAR * const UnicodeString) {
    //#[ operation FromUnicodeString(WCHAR * const) 
    std::string result = "";

    int stringLength = WideCharToMultiByte(CP_UTF8, 0, UnicodeString, -1, NULL, 0, NULL, NULL); 
    
    if(stringLength == 0) 
    	return "";
    else
    {
    	char * txtNarrow = new char[stringLength]; 
    	txtNarrow[0] = '\0';
    	
    	int intTxtLength = WideCharToMultiByte(	CP_UTF8, 
    										0, 
    										UnicodeString, 
    										-1, 
    										txtNarrow, 
    										stringLength, 
    										0, 
    										0);        
    	
		for(register int i = 0; i < intTxtLength-1; ++i) result += txtNarrow[i];
    	
    	delete[] txtNarrow;          
    	
    	
    }
    return result;
    //#]
}
int ToUnicodeString(std::string OriginalString, WCHAR * const UnicodeString, size_t MaxLengthInWChars) {
    //#[ operation ToUnicodeString(OMString,WCHAR * const,size_t) 
    
    //Try to convert using UTF-8 else fallback to using ANSI    
    int txtLength = MultiByteToWideChar(	CP_UTF8, 
    					0, 
						&OriginalString.at(0), 
    					-1, 
    					UnicodeString, 
    					static_cast<int>(MaxLengthInWChars));
    									
    return txtLength - 1;   
    
    //#]
}
void createXML(std::string contents)
{

	DWORD dwBytesWritten;
	const wchar_t* filePath = L"C:\\Users\\matthew.smith\\Desktop\\Test\\Documentation.xml";


	HANDLE hFile = CreateFile(filePath,
									GENERIC_WRITE,
									0,
									NULL,
									CREATE_NEW,
									FILE_ATTRIBUTE_NORMAL,
									NULL
									);
	//Make sure we create the file
	if(NULL != hFile)
	{
		char * data = &contents.at(0);
		WriteFile(hFile, "<STAND_PLANS>",13, &dwBytesWritten,false);
		WriteFile(hFile, data,contents.size(), &dwBytesWritten,false);
		WriteFile(hFile, "</STAND_PLANS>",14, &dwBytesWritten,false);
	}
	
	//Close out file handle
    CloseHandle(hFile);
    
}

void searchFolder(std::string strFolderName, const HANDLE &hFind, WIN32_FIND_DATA &ffd)
{
	std::string strContents = "";
	while (FindNextFile(hFind, &ffd) != 0)
	{
		// Not intrested in foldres just files
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
		{
			strContents += "<STAND_PLAN_NAME>" + FromUnicodeString(ffd.cFileName) + "</STAND_PLAN_NAME>";
			createXML(strContents);
		}

	}

}
std::string FindFile(const std::string & Folder, const std::string & RequiredExtension, const int & Index) {
    //#[ operation FindFile(const OMString &,const OMString &,const int &,OMBoolean) 
    
    std::string strFilename = "";
    
    HANDLE hFindFile       ;
    WIN32_FIND_DATA fdData ;
    WCHAR wstrSearchPath[MAX_PATH];
    bool blnMatch = true;
    
    //Convert the filename to a unicode string                                                
	//bool blnAddSlash = Folder.at(0)[Folder.size() - 1] != '\\';                                              
	std::string strSearchPath = Folder + /*(blnAddSlash ? std::string("\\") :*/ std::string("")/*)*/ + "*." + RequiredExtension;
    ToUnicodeString(strSearchPath, wstrSearchPath, MAX_PATH);
    
    int intCurrentIndex = 0;
    bool blnFirst = true;
    
    do
    {
    	//Clear our filename   
    	strFilename = "";
    	
    	//Check to see if the first match exists
    	if(blnFirst)
    	{
    		//Find the first file
    		hFindFile = FindFirstFile(wstrSearchPath, &fdData);
    		blnMatch = (hFindFile != INVALID_HANDLE_VALUE);
    		blnFirst = false;
    	}
    	else 
    	{
    		//Find subsequent file
    		blnMatch = (FindNextFile(hFindFile, &fdData) != 0);		
    	}
    	
    	//Make sure this isn't a directory entry
    	if( blnMatch && ((fdData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) )
    	{
    		//Extract the filename from the find data structure
    		strFilename = FromUnicodeString(fdData.cFileName);				
    	
    		//Do we need to remove the extension?
    		//if(!IncludeExtInResult) strFilename = clsStringManipulator::SubString(strFilename, 0, clsStringManipulator::LastIndexOf(strFilename, "."));
    		
    		//Move on to the next file	
    		intCurrentIndex++;
    	}	
    	
    }while( (intCurrentIndex <= Index) && blnMatch );
    
    //Close our file find handle
    FindClose(hFindFile); 
    
    //Return the filename
    return strFilename;
    
    //#]
}
std::string FindFolder(const std::string & Folder, const int & Index) {
    //#[ operation FindFolder(const OMString &,const int &) 
    
    std::string strFolderName = "";
    
    HANDLE hFindFile       ;
    WIN32_FIND_DATA fdData ;
    WCHAR wstrSearchPath[MAX_PATH];
    bool blnMatch = true;
                        
    //Convert the filename to a unicode string
    std::string strSearchPath = Folder + "*.*";
    ToUnicodeString(strSearchPath, wstrSearchPath, MAX_PATH);
    
    int intCurrentIndex = 0;
    bool blnFirst = true;
    
    do
    {
    	//Clear our filename   
    	strFolderName = "";
    	
    	//Check to see if the first match exists
    	if(blnFirst)
    	{
    		//Find the first file
    		hFindFile = FindFirstFile(wstrSearchPath, &fdData);
    		blnMatch = (hFindFile != INVALID_HANDLE_VALUE);
    		blnFirst = false;
    	}
    	else 
    	{
    		//Find subsequent file
    		blnMatch = (FindNextFile(hFindFile, &fdData) != 0);		
    	}
    	
    	//Make sure this isn't a directory entry
    	if( blnMatch && ((fdData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) )
    	{
    		//Extract the filename from the find data structure
    		strFolderName = FromUnicodeString(fdData.cFileName);				
    	
    		//Move on to the next file	
    		++intCurrentIndex;
    	}	
    	
    }while( (intCurrentIndex <= Index) && blnMatch );
    
    //Close our file find handle
    FindClose(hFindFile); 
    
    //Return the filename
    return strFolderName;
    
    //#]
}
int main()
{

	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	bool blnFirst = false;
	std::string stdFolderName  = "";

	int  i = 2;
	int  j = 2;
	while ( (stdFolderName = FindFolder("C:\\Users\\matthew.smith\\Desktop\\Test\\DOCUMENTATION\\*",i)) != "" )
	{
		//searchFolder(stdFolderName,hFind,ffd);
		//FindFolder("C:\\Users\\matthew.smith\\Desktop\\Test\\DOCUMENTATION\\"+stdFolderName + "\\*",j);
		i++;
	}
	FindFile("C:\\Users\\matthew.smith\\Desktop\\Test\\DOCUMENTATION\\","",0);



//	std::string strContents = "";

	hFind = FindFirstFile(L"C:\\Users\\matthew.smith\\Desktop\\Test\\DOCUMENTATION\\*.",&ffd);//\\STAND_PLANS\\*",&ffd);
	if (INVALID_HANDLE_VALUE == hFind) 
	{
		return dwError;
	} 
	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0 )
		{
			//if("." != FromUnicodeString(ffd.cFileName))
			{
				_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			//	searchFolder(FromUnicodeString(ffd.cFileName),hFind,ffd);
			}
			// Directory found 
				// search found directory 
				// While not End of FOLDER
					// If file found 
						// Process file
			// Move to next directory


		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			//strContents += "<STAND_PLAN_NAME>" + FromUnicodeString(ffd.cFileName) + "</STAND_PLAN_NAME>";
		}
	}
   while (blnFirst = FindNextFile(hFind, &ffd) != 0);
   //createXML(strContents);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
	   return dwError;
   }

   FindClose(hFind);
   return dwError;

}