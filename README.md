# OCR in C++ using OpenCV and Tesseract Engine
## **Description**
The Optical Character Recognition (OCR) project is a robust and versatile application developed in C++ utilizing the OpenCV library. This project aims to enable the extraction of textual information from images or documents, facilitating the conversion of scanned documents, images, or camera-captured snapshots into editable and searchable text formats.
## **Integration of libraries**
 ### **OpenCV**:
- Initially you need to download the OpenCV into your personal computer, you can install it from the provided link or you can download the updated version according to your requirements. (https://github.com/opencv/opencv/releases/tag/4.5.1)
- Then you need to install it into your PC.
- After a successfull installation of OpenCV you need to include directories of your OpenCV folder into your project along with libraries and dependencies.
- In my case, I am using visual studio community 2022, tally the process with respective IDE's.
+ **To include headers and Library**:
+ Go to your **project properties**, and then to **VC++ directories**.
+ Go to **include directories** and add path: **D:\opencv\build\include**, **D** refers to the disk containing OpenCV.
+ Go to **Libraries directories** and then add path: **D:\opencv\build\x64\vc15\lib**.
+ Navigate into Linker, then to input and add additional dependencies which will be located in your lib folder such as **opencv_world451d.lib**.
 ### **Tesseract Engine**:
- In the beginning of the project, my intention was to include tesseract library into my project in order to make it efficient through vcpkg but after months of struggle and research from different platforms, reading articles and watching videos and unsuccessful attempts I almost gave up and used tesseract engine instead. (https://tesseract-ocr.github.io/tessdoc/Examples_C++.html). 
- I would prefer to go and check this forum if you want to use tesseract library along with your project.(https://stackoverflow.com/questions/50658953/install-tesseract-for-c-on-windows-10)
- Add path of the tesseract folder to your computer environment variable so that cammand prompt get access to OCR.
- If you want to make sure tesseract is successfully installed write in cmd **"tesseract --help"**.
 ### **Integrating wxWidget**:
- At first you need to download zip file of wxWidget from its official website and then navigate to downloads.(https://www.wxwidgets.org/downloads/)
- Extract the zip file into some destination. Letter, navigate to **WxWidget->build->msw->wxvc17.sln**, build it for x32, and x64 it will generate files name **vc_x64_mswu** and **vc_x64_mswud** into the build folder. Note build it with both debug and release configuration.
- Add path of wxwidget folder to your environment variable the way you did with your tesseract OCR engine.
- Go to your IDE and open a new empty project. Navigate to properties of your project. Make sure **configuration=all configuration** and **platform=all platform**.
- Navigate to **C++ -> Additional include directories** then add path to include headers such that **(E:\WXwidgets\include\msvc;E:\WXwidgets\include)**.
- Navigate to **Linker->System** and change subsystem from console to windows.
- Navigate to **Linker->General** then add path to additional library directories according to your system requirements such as **E:\WXwidgets\lib\vc_x64_lib** in case of x32, remember I build files for both x32 and x64. You can add any of each according to your personal requirements.
- Then copy the code from this website if your wxWidget is working fine.(https://pastebin.com/9rb3bcyx).
 ###**GUI and User Interface**
-![Graphical User Interface of OCR](GUI.PNG).

### **Thank You!**
