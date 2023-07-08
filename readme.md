CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
    * Windows
    * Linux
 * Configuration
    * App settings
    * Change DEMoS server URL
 * Synchronization
 * Documentation
 * Repository
 * Authors
 
 
 INTRODUCTION
 ------------
 
This file contains information about program ProGenealogy with the extension allowing synchronization with system DEMoS (www.genealogickadatabaze.cz or radegast.fit.vutbr.cz) and exporting data to graph database. 
Application ProGenealogy provides features such as viewing and editing genealogical data in GEDCOM format and constructing and 
exporting family tree diagrams. It is a multi-platform application. 

 
 
 REQUIREMENTS
 ------------
 
 Application requirements for Windows:
 
 * Windows 64-bit
 
 Application requirements for Linux:
 
 * Installed Qt 5.15.2
 * Installed QtCreator 4.9.0 or higher
 
 
 INSTALLATION
 ------------
 
 * Windows:
    
    1. Copy the folder bin/windows/ to a folderin your computer
    2. ProGenealogy.exe is ready to be executed
 
 
 * Linux:
 
    1. Open src/Application.pro in QtCreator and build the project
    2. Run created executable
 
 
 CONFIGURATION
 -------------
 * App settings

   Customize the application in Menu > Settings

 * Change DEMoS server URL

   DEMoS database server URL is defined in `DEMOS_API_ADDRESS` in `\src\kernel\record\recordRAM\primitives.h`. For change to take effect, the app must be rebuilt.

 SYNCHRONIZATION
 ---------------

 For synchronization with the system DEMoS, you will need to have an account. You can mark records in web interface of the system, what records will be imported by marking them in the system. While synchronizing in the application, use same credentials you have in DEMoS.

 For export to graph database, you will need an Neo4j database avialable. Neo4j server can be downloaded at https://neo4j.com/download-center/

Notes: 
 * During synchronization with the system DEMoS, database records are only imported, not exported. Only information, that is exported is information about whether records are marked as imported or not.
 * Neo4j database for export of data, must support communication using HTTP protocol.
 
 DOCUMENTATION
 -------------
 
 The Doxygen documentation:
 
 * Requires installed doxygen
 * Generated by command "make doc" in src/
 * Can be found in doc/doxygen
    
    
 The technical report
 
 * Technical report on implementation of the extension for synchronization can be found in doc/thesis. For technical report on implementation of the application look in the orignal repository in section 'REPOSITORY'. 
 
 
 REPOSITORY
 ----------
 
Original application created by Eva Navratilova:
	https://bitbucket.org/Shina13/progenealogy/src/master
Application with database synchronization extension:
	https://github.com/xnemce07/ProGenealogy
 
 
 AUTHORS
 -------
 
 Author of the application:
 
 * Eva Navratilova
 	* xnavra54@stud.fit.vutbr.cz
 	* navratie@gmail.com

 Author of the extension for DEMoS synchronization and export to graph database:

 * Leopold Nemcek
	* xnemce07@stud.fit.vutbr.cz
 
 
 
