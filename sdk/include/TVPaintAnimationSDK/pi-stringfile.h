/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-stringfile.h
*  @ingroup  sdk_TVPA
*
*  @brief  Access to configuration files.
*/

#ifndef __pi_stringfile_HH
#define __pi_stringfile_HH


#include "TVPaintAnimationSDK/pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIStringFile  Access to configuration files.
*/


//===============================================================================================================


///@name  String Files/Configuration file.
///@{

/** @brief  Opens a configuration file.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iFileName  The file's path/name.
    @param[in]  iFlags     1 = create the file, 0 - open existing file.

    @return  Opaque handle to the file, NULL on failure.

    @see  TVCloseStringFile(), TVReadStringFile(), TVWriteStringFile(), TVEraseStringFileSection()

    @ingroup  sdk_PIStringFile
*/
void*  TVOpenStringFile( PIPlugin*  iPlugin, const char*  iFileName, PIFlags  iFlags );


/** @brief  Closes a configuration file.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iFile    The file's handle.
    @param[in]  iFlags   Unused (pass 0).

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVOpenStringFile(), TVReadStringFile(), TVWriteStringFile(), TVEraseStringFileSection()

    @ingroup  sdk_PIStringFile
*/
int  TVCloseStringFile( PIPlugin*  iPlugin, void*  iFile, PIFlags  iFlags );


/** @brief  Reads a string from a configuration file.

    @param[in]   iPlugin   This plugin's reference.
    @param[in]   iFile     The file's handle.
    @param[in]   iSection  Section in which the string has to be read. Often, the plug-in name is used.
    @param[in]   iName     Name of the string to read.
    @param[out]  oString   The result string.
    @param[in]   iDefault  Default string, if the entry does not exist in the configuration file.
    @param[in]   iMaxChar  Size of oString. No more than iMaxChar-1 will be read.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVOpenStringFile(), TVCloseStringFile(), TVWriteStringFile(), TVEraseStringFileSection()

    @ingroup  sdk_PIStringFile
*/
int  TVReadStringFile( PIPlugin*  iPlugin, void*  iFile, const char*  iSection, const char*  iName, char*  oString, const char*  iDefault, int  iMaxChar );


/** @brief  Writes a string into a configuration file.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iFile     The file's handle.
    @param[in]  iSection  Section in which the string has to be written. Often, the plug-in name is used.
    @param[in]  iName     Name of the string to write.
    @param[in]  iString   The string to write.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVOpenStringFile(), TVCloseStringFile(), TVReadStringFile(), TVEraseStringFileSection()

    @ingroup  sdk_PIStringFile
*/
int  TVWriteStringFile( PIPlugin*  iPlugin, void*  iFile, const char*  iSection, const char*  iName, const char*  iString );


/** @brief  Erases a whole section from a configuration file.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iFile     The file's handle.
    @param[in]  iSection  The section to erase.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVOpenStringFile(), TVCloseStringFile(), TVReadStringFile(), TVWriteStringFile()

    @ingroup  sdk_PIStringFile
*/
int  TVEraseStringFileSection( PIPlugin*  iPlugin, void*  iFile, const char*  iSection );

///@}


//===============================================================================================================


///@name  String Files/Preferences file.
///@{

/** @brief  Reads a string from the user preferences file.

    @param[in]   iPlugin   This plugin's reference.
    @param[in]   iSection  Section in which the string has to be read. Often, the plug-in name is used.
    @param[in]   iName     Name of the string to read.
    @param[out]  oString   The result string.
    @param[in]   iDefault  Default string, if the entry does not exist in the configuration file.
    @param[in]   iMaxChar  Size of oString. No more than iMaxChar-1 will be read.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVWriteUserString(), TVEraseUserSection()

    @ingroup  sdk_PIStringFile
*/
int  TVReadUserString( PIPlugin*  iPlugin, const char*  iSection, const char*  iName, char*  oString, const char*  iDefault, int  iMaxChar );


/** @brief  Writes a string into the user preferences file.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iSection  Section in which the string has to be written. Often, the plug-in name is used.
    @param[in]  iName     Name of the string to write.
    @param[in]  iString   The string to write.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVReadUserString(), TVEraseUserSection()

    @ingroup  sdk_PIStringFile
*/
int  TVWriteUserString( PIPlugin*  iPlugin, const char*  iSection, const char*  iName, const char*  iString );


/** @brief  Erases a whole section from the user preferences file.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iSection  The section to erase.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVReadUserString(), TVWriteUserString()

    @ingroup  sdk_PIStringFile
*/
int  TVEraseUserSection( PIPlugin*  iPlugin, const char*  iSection );

///@}


//===============================================================================================================


///@name  String Files/Project file.
///@{

/** @brief  Reads a string from the project's configuration.

    @param[in]   iPlugin   This plugin's reference.
    @param[in]   iSection  Section in which the string has to be read. Often, the plug-in name is used.
    @param[in]   iName     Name of the string to read.
    @param[out]  oString   The result string.
    @param[in]   iDefault  Default string, if the entry does not exist in the configuration file.
    @param[in]   iMaxChar  Size of oString. No more than iMaxChar-1 will be read.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVWriteProjectString(), TVEraseProjectSection()

    @ingroup  sdk_PIStringFile
*/
int  TVReadProjectString( PIPlugin*  iPlugin, const char*  iSection, const char*  iName, char*  oString, const char*  iDefault, int  iMaxChar );


/** @brief  Writes a string into the project's configuration.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iSection  Section in which the string has to be written. Often, the plug-in name is used.
    @param[in]  iName     Name of the string to write.
    @param[in]  iString   The string to write.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVReadProjectString(), TVEraseProjectSection()

    @ingroup  sdk_PIStringFile
*/
int  TVWriteProjectString( PIPlugin*  iPlugin, const char*  iSection, const char*  iName, const char*  iString );


/** @brief  Erases a whole section from the project's configuration.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iSection  The section to erase.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVReadProjectString(), TVWriteProjectString()

    @ingroup  sdk_PIStringFile
*/
int  TVEraseProjectSection( PIPlugin*  iPlugin, const char*  iSection );

///@}


#ifdef __cplusplus
}
#endif


#endif // __pi_stringfile_HH

