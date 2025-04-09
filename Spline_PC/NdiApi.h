#pragma once
#pragma comment(lib,"library.lib")
#ifdef _WIN32
// For Windows Sleep(ms)
#include <windows.h>
#else
// For POSIX sleep(sec)
#include <unistd.h>
#endif
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <CombinedApi.h>
#include <PortHandleInfo.h>
#include <ToolData.h>
#include <QString>
#include <QDebug>

class NdiApi
{
public:
	NdiApi();

	CombinedApi capi = CombinedApi();
	bool apiSupportsBX2 = false;
	double Qx1 = -1.0;
	double Qy1 = -1.0;
	double Qz1 = -1.0;
	double Qo1 = -1.0;
	double Tx1 = -1.0;
	double Ty1 = -1.0;
	double Tz1 = -1.0;
	double err1 = -1.0;

	double Qx2 = -1.0;
	double Qy2 = -1.0;
	double Qz2 = -1.0;
	double Qo2 = -1.0;
	double Tx2 = -1.0;
	double Ty2 = -1.0;
	double Tz2 = -1.0;
	double err2 = -1.0;

	double Qx3 = -1.0;
	double Qy3 = -1.0;
	double Qz3 = -1.0;
	double Qo3 = -1.0;
	double Tx3 = -1.0;
	double Ty3 = -1.0;
	double Tz3 = -1.0;
	double err3 = -1.0;

	void sleepSeconds(unsigned numSeconds);

	void onErrorPrintDebugMessage(std::string methodName, int errorCode);
	std::string getToolInfo(std::string toolHandle);
	std::string toolDataToCSV(const ToolData& toolData);
	void writeCSV(std::string fileName, int numberOfLines);
	void printToolData(const ToolData& toolData);
	void printTrackingData();
	void initializeAndEnableTools();
	void loadTool(std::string toolDefinitionFilePath);
	void configurePassiveTools();
	void configureActiveTools(std::string scuHostname);
	void configureActiveWirelessTools();
	void configureDummyTools();
	void configureUserParameters();
	void simulateAlerts(uint32_t simulatedAlerts);
	void determineApiSupportForBX2();

	int registerTools();

};

