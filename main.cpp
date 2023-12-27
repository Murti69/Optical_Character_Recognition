#include <wx/wx.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/filedlg.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include "ImgProc.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

vector<Point> InitialPoints, DocPoints;

float w = 700, h = 600;

Mat Warping(Mat Img, vector<Point> points, float w, float h)
{
    Point2f src[4] = { points[0],points[1],points[2],points[3] };
    Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };
    Mat Matrix = getPerspectiveTransform(src, dst);
    warpPerspective(Img, Img, Matrix, Point(w, h));
    return Img;
}

vector<Point> ReOrder(vector<Point> points)
{
    vector<Point> newPoints;
    vector<int> SumPoints, SubPoints;
    for (int i = 0; i < 4; i++)
    {
        SumPoints.push_back(points[i].x + points[i].y);
        SubPoints.push_back(points[i].x - points[i].y);
    }
    newPoints.push_back(points[min_element(SumPoints.begin(), SumPoints.end()) - SumPoints.begin()]);//0
    newPoints.push_back(points[max_element(SubPoints.begin(), SubPoints.end()) - SubPoints.begin()]);//1
    newPoints.push_back(points[min_element(SubPoints.begin(), SubPoints.end()) - SubPoints.begin()]);//2
    newPoints.push_back(points[max_element(SumPoints.begin(), SumPoints.end()) - SumPoints.begin()]);//3

    return newPoints;
}

void drawPoints(vector<Point> points, Scalar color,Mat OriginalImg)
{
    for (int i = 0; i < points.size(); i++)
    {
        circle(OriginalImg, points[i], 10, color, FILLED);
        putText(OriginalImg, to_string(i), points[i], FONT_HERSHEY_PLAIN, 3, color, 3);
    }
}

vector<Point> GetContours(Mat Img,Mat OriginalImg)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    int MaxArea = 0;

    findContours(Img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //drawContours(Img,contours,-1,Scalar(255,0,255),2);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    vector<Point> biggest;
    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;

        string objectType;
        if (area > 2000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if (area > MaxArea && conPoly[i].size() == 4)
            {
                drawContours(OriginalImg, conPoly, i, Scalar(255, 0, 255), 2);
                biggest = { conPoly[i][0],conPoly[i][1], conPoly[i][2], conPoly[i][3] };
                MaxArea = area;
            }

        }
    }
    return biggest;
    //It will return initial points.
    //we need to reorder these initial points before drawing contour
}


class GradientButton : public wxButton {
public:
    GradientButton(wxWindow* parent, wxWindowID id, const wxString& label)
        : wxButton(parent, id, label) {
        SetBackgroundColour(*wxBLUE);
        SetForegroundColour(*wxWHITE);

    }
};

class OCRTextAndImageDisplay : public wxFrame {
public:
    OCRTextAndImageDisplay(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
        mainSizer->Add(textCtrl, 1, wxEXPAND | wxALL, 10);

        imageCtrl = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
        mainSizer->Add(imageCtrl, 1, wxEXPAND | wxALL, 10);

        SetSizerAndFit(mainSizer);
    }

    wxTextCtrl* GetTextCtrl() { return textCtrl; }
    wxStaticBitmap* GetImageCtrl() { return imageCtrl; }

private:
    wxTextCtrl* textCtrl;
    wxStaticBitmap* imageCtrl;
};


class OCRApp : public wxFrame {
public:
    OCRApp(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
        // Set background color
        SetBackgroundColour(wxColour(173, 216, 230)); // Light blue color

        // Create text label
        wxStaticText* textLabel = new wxStaticText(this, wxID_ANY, "SCHOOL OF ELECTRICAL ENGINEERING AND COMPUTER SCIENCE (SEECS)");
        wxStaticText* textLabel1 = new wxStaticText(this, wxID_ANY, "END SEMESTER PROJECT FALL 2023 CS-212 (OOP)");
        wxStaticText* textLabel2 = new wxStaticText(this, wxID_ANY, "PRESENTED BY M MURTAZA BAIG AND M UMAIR AJMAL");
        textLabel->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        textLabel1->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        textLabel2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        textLabel->SetForegroundColour(*wxBLACK);
        textLabel1->SetForegroundColour(*wxBLACK);
        textLabel2->SetForegroundColour(*wxBLACK);

        // Create the buttons without specifying size
        GradientButton* cameraButton = new GradientButton(this, wxID_ANY, "Open Camera");
        GradientButton* uploadButton = new GradientButton(this, wxID_ANY, "Upload Image");

        // Set the size for all buttons
        wxSize buttonSize(200, 50); // Adjust the size according to your preference
        cameraButton->SetSize(buttonSize);
        uploadButton->SetSize(buttonSize);
        

        // Enlarge the buttons' text
        wxFont enlargedFont = cameraButton->GetFont().Scaled(2);
        cameraButton->SetFont(enlargedFont);
        uploadButton->SetFont(enlargedFont);
       

        // Bind button events
        cameraButton->Bind(wxEVT_BUTTON, &OCRApp::OnCameraButtonClick, this);
        uploadButton->Bind(wxEVT_BUTTON, &OCRApp::OnUploadButtonClick, this);
        


      
        // Create a sizer and add widgets
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(textLabel, 0, wxALIGN_CENTER | wxALL, 10);
        mainSizer->Add(textLabel1, 0, wxALIGN_CENTER | wxALL, 10);
        mainSizer->Add(textLabel2, 0, wxALIGN_CENTER | wxALL, 10);
        mainSizer->Add(cameraButton, 0, wxALIGN_CENTER | wxALL, 10);
        mainSizer->Add(uploadButton, 0, wxALIGN_CENTER | wxALL, 10);
        
        SetSizerAndFit(mainSizer);
    }

private:
    void OnCameraButtonClick(wxCommandEvent& event) {
        wxLogMessage("Opened Camera");

        cv::VideoCapture cap(0);
        cv::Mat img,PreprocessedImg,WarpImg,ResizeImg;

        while (true) {
            cap.read(img);
            cv::imshow("img", img);

            int key = cv::waitKey(1); // Wait for a key event (1 millisecond delay)

            if (key == 'c' || key == 'C') {
                cv::imwrite("capture.jpg", img);
                break; // Break the loop when 'c' or 'C' key is pressed
            }
        }
        //resize(img, ResizeImg, Size(), 0.8, 0.8);
        cvtColor(img, PreprocessedImg, COLOR_BGR2GRAY);

        threshold(PreprocessedImg, PreprocessedImg, 0, 255, THRESH_BINARY | THRESH_OTSU);

        equalizeHist(PreprocessedImg, PreprocessedImg);
        cv::imshow("captured Img", img);
        imshow("Image", PreprocessedImg);
        waitKey(0);

        imwrite("CapturedToBeProcessed.png", PreprocessedImg);
        string filePath;
        filePath = "C:\\Users\\HP\\source\\repos\\OCR\\CapturedToBeProcessed.png";
        string tesseract = "tesseract \"";
        string command = tesseract + filePath + "\" OCR_Cap.txt -l eng";

        int result = system(command.c_str());

    }


    void OnUploadButtonClick(wxCommandEvent& event) {
        // Open file dialog to select an image file
        wxFileDialog openFileDialog(this, wxT("Open Image File"), wxT(""), wxT(""), wxT("Image Files (*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return; // User canceled the dialog

        wxString filePath = openFileDialog.GetPath();
        std::string filePathStr = std::string(filePath.mb_str());

        cv::Mat Img, input_Image, ResizeImg, PreprocessedImg, WarpImg, CropImg;
        vector<Point> InitialPoints, docPoints;

        Img = imread(filePathStr);
       
        resize(Img, ResizeImg, Size(), 0.75, 0.75);
        Img = ResizeImg;
        cvtColor(ResizeImg, PreprocessedImg, COLOR_BGR2GRAY);
        /*imshow("Image Gray", PreprocessedImg);
        waitKey(0);*/
        GaussianBlur(PreprocessedImg, PreprocessedImg, Size(3, 3), 3, 0);
        //adaptiveThreshold(PreprocessedImg, PreprocessedImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
        Canny(PreprocessedImg, PreprocessedImg, 25, 75);
        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        dilate(PreprocessedImg, PreprocessedImg, kernel);
        
        InitialPoints = GetContours(PreprocessedImg, Img);
        //drawPoints(InitialPoints, Scalar(255, 0, 0), ResizeImg);
         // Check if no boundaries are detected (i.e., if no contour points are found or the points are too few)
        if (InitialPoints.empty() || InitialPoints.size() < 4) {

            cvtColor(Img, PreprocessedImg, COLOR_BGR2GRAY);
            threshold(PreprocessedImg, PreprocessedImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
            equalizeHist(PreprocessedImg, PreprocessedImg);
            imwrite("ImageToBeProcessed.png", PreprocessedImg);
            
            resize(Img, Img, Size(500,350));
        }
        else {
            DocPoints = ReOrder(InitialPoints);
            WarpImg = Warping(ResizeImg, DocPoints, w, h);

            // Crop the image
            Rect roi(5, 5, w - (2 * 5), h - (2 * 5));
            CropImg = WarpImg(roi);

            cvtColor(CropImg, CropImg, COLOR_BGR2GRAY);
            threshold(CropImg, CropImg, 0, 255, THRESH_BINARY | THRESH_OTSU);

            equalizeHist(CropImg, CropImg);
            imwrite("ImageToBeProcessed.png", CropImg);

            resize(Img, Img, Size(500, 350));

        }

        string filepath;


        filepath = "C:\\Users\\HP\\source\\repos\\OCR\\ImageToBeProcessed.png";

        string tesseract = "tesseract \"";
        string command = tesseract + filepath + "\" OCR.txt -l eng";

        int result = system(command.c_str());
        
        // Open the child window to display OCR text and processed image
        OCRTextAndImageDisplay* displayWindow = new OCRTextAndImageDisplay("OCR Text and Processed Image");
        displayWindow->Show();

        // Get text and image controls from the child window
        wxTextCtrl* textCtrl = displayWindow->GetTextCtrl();
        wxStaticBitmap* imageCtrl = displayWindow->GetImageCtrl();

        // ... existing code to load and process the image ...

        // Display the processed image
        wxImage wxImg = wxImage(Img.cols, Img.rows, Img.data, true);
        wxBitmap bitmap = wxBitmap(wxImg);
        imageCtrl->SetBitmap(bitmap);

        //To read the text file
        string filePathOCR = "C:\\Users\\HP\\source\\repos\\OCR\\OCR.txt.txt";

        // Read OCR text from the file
        ifstream ocrFile(filePathOCR);
        string ocrText;
        if (ocrFile.is_open()) {
            stringstream buffer;
            buffer << ocrFile.rdbuf();
            ocrText = buffer.str();
            ocrFile.close();
        }
        else {
            wxMessageBox("Failed to open the OCR file!", "Error", wxICON_ERROR | wxOK);
            return;
        }

        // Check if the OCR text is empty
        if (ocrText.empty()) {
            wxMessageBox("Empty OCR text!", "Error", wxICON_ERROR | wxOK);
            return;
        }

        // Set the OCR text in the text control of the existing window
        if (textCtrl)
            textCtrl->SetValue(wxString(ocrText.c_str(), wxConvUTF8));


    }
    void OnUploadPdfButtonClick(wxCommandEvent& event) {
        // Handle the "Upload PDF" button click event
        // Add the functionality to handle PDF upload here
        wxFileDialog openFileDialog(this, wxT("Open PDF File"), wxT(""), wxT(""), wxT("PDF Files (*.pdf)|*.pdf"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return; // User canceled the dialog

        wxString filePath = openFileDialog.GetPath();
        string filePathStr =string(filePath.mb_str());

        // Add your logic to process the uploaded PDF here
        // ...

        wxMessageBox("PDF Upload functionality to be implemented!", "Information", wxOK | wxICON_INFORMATION);
    }
    wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(OCRApp, wxFrame)
EVT_BUTTON(wxID_ANY, OCRApp::OnCameraButtonClick)
EVT_BUTTON(wxID_ANY, OCRApp::OnUploadButtonClick)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    bool OnInit() override {
        OCRApp* ocrApp = new OCRApp("OCR App");
        ocrApp->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
