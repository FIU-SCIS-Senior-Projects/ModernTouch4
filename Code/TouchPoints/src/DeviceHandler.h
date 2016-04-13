#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "libusb.h"
#include "UserInterface.h"
#include "Enums.h"
#include <stdio.h>

struct DeviceHandler{

	//Creates a Device Handlers
	DeviceHandler(){
		leapConnected = 0;
		multiTouchConnected = 0;
		eyeXConnected = 0;
		realSenseConnected = 0;
		leapConnectedFlag = 0;
		multiTouchConnectedFlag = 0;
		eyeXConnectedFlag = 0;
		stateCounter = 0;
		vendorList[0] = 61826;
		vendorList[1] = 8746;
		vendorList[2] = 8452;
		vendorList[3] = 16537; //1054;
		overrideLeap = false;
		overrideMultiTouch = false;
		overrideEyeX = false;
		overrideRealSense = false;
		leapDrawEnabled = true;
		leapGestureEnabled = true;
	}


	int deviceConnection();
	Mode::DefaultModes getDefaultMode();
	int leapStatus();
	int multiTouchStatus();
	int eyeXStatus();
	int realSenseStatus();

	
	void toggleMultiTouch();
	void toggleEyeX();
	//Sense Device and Functionality
	void toggleRealSense();
	void toggleRealSenseExpressions();

	bool realSenseExpressions();
	//Leap Device and Functionality
	void toggleLeap();
	void toggleLeapDraw();
	void toggleLeapGesture();

	bool leapDraw();
	bool leapGesture();

	Mode::DefaultModes set_MLER_Mode();
	Mode::DefaultModes set_MLE_Mode();
	Mode::DefaultModes set_MLR_Mode();
	Mode::DefaultModes set_MER_Mode();
	Mode::DefaultModes set_LER_Mode();
	Mode::DefaultModes set_ML_Mode();
	Mode::DefaultModes set_ME_Mode();
	Mode::DefaultModes set_MR_Mode();
	Mode::DefaultModes set_LE_Mode();
	Mode::DefaultModes set_LR_Mode();
	Mode::DefaultModes set_ER_Mode();
	Mode::DefaultModes set_L_Mode();
	Mode::DefaultModes set_E_Mode();
	Mode::DefaultModes set_M_Mode();
	Mode::DefaultModes set_R_Mode();
	

private:
	int setLeapState();
	int setMultiTouchState();
	int setEyeXState();
	int setRealSenseState();
	void resetFlags();



	int leapConnectedFlag;
	int multiTouchConnectedFlag;
	int eyeXConnectedFlag;
	int realSenseConnectedFlag;
	int stateCounter;
	int vendorList[4];

	//Mode Flags
	int leapConnected;
	int multiTouchConnected;
	int eyeXConnected;
	int realSenseConnected;
	bool realSenseExpressionsEnabled;
	bool leapDrawEnabled;
	bool leapGestureEnabled;
	bool eyeXEnabled;
	bool overrideLeap;
	bool overrideMultiTouch;
	bool overrideEyeX;
	bool overrideRealSense;

};



bool DeviceHandler::leapDraw(){
	
	if (leapConnected){
		return leapDrawEnabled;
	}
	else {
		return false;
	}
}
bool DeviceHandler::leapGesture(){
	if (leapConnected){
		return leapGestureEnabled;
	}
	else {
		return false;
	}
}
bool DeviceHandler::realSenseExpressions(){
	if (realSenseConnected){
		return realSenseExpressionsEnabled;
	}
	else {
		return false;
	}

}

void DeviceHandler::toggleLeap(){

	if (leapConnected){
		leapConnected = false;
		overrideLeap = true;
	}
	else {
		overrideLeap = false;
		deviceConnection();
	}
	//overrideLeap = true;
	/*
	leapConnected = !leapConnected;
	if (leapConnected){
		overrideLeap = false;
	}
	else {
		overrideLeap = true;
	}
	*/
}
void DeviceHandler::toggleLeapDraw(){

	leapDrawEnabled = !leapDrawEnabled;
}
void DeviceHandler::toggleLeapGesture(){

	leapGestureEnabled = !leapGestureEnabled;

}
void DeviceHandler::toggleRealSense(){
	//realSenseConnected = !realSenseConnected;
	if (realSenseConnected){
		realSenseConnected = false;
		overrideRealSense = true;
	}
	else {
		overrideRealSense = false;
		deviceConnection();
	}
}
void DeviceHandler::toggleRealSenseExpressions(){
	realSenseExpressionsEnabled = !realSenseExpressionsEnabled;
}
void DeviceHandler::toggleMultiTouch(){
	if (multiTouchConnected){
		multiTouchConnected = false;
		overrideMultiTouch = true;
	}
	else {
		overrideMultiTouch = false;
		deviceConnection();
	}
	/*
	multiTouchConnected = !multiTouchConnected;
	if (multiTouchConnected){
		overrideMultiTouch = false;
	}
	else {
		overrideMultiTouch = true;
	}
	*/
}
void DeviceHandler::toggleEyeX(){

	if (eyeXConnected){
		eyeXConnected = false;
		overrideEyeX = true;
	}
	else {
		overrideEyeX = false;
		deviceConnection();
	}
	/*
	eyeXConnected = !eyeXConnected;
	if (eyeXConnected){
		overrideEyeX = false;
	}
	else {
		overrideEyeX = true;
	}
	*/
	
}


int DeviceHandler::deviceConnection(){

	libusb_device **devs;			//pointer to pointer of device, used to retrieve a list of devices
	libusb_context *contex = NULL;		//a libusb session

	int r;			//return values
	ssize_t listCount;	//Number of devices in list

	//Intialize Library Session
	r = libusb_init(&contex);
	if (r < 0){
		//Intialization Error
		return -1;
	}

	//Get the list of devices
	listCount = libusb_get_device_list(contex, &devs);
	if (listCount < 0) {
		return (int)listCount;
	}

	libusb_device *dev;

	int i = 0, j = 0;
	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			//Error
			return -1;
		}

		uint16_t vendorid = desc.idVendor;
		uint16_t productid = desc.idProduct;

		if (vendorid == vendorList[0]){
			leapConnectedFlag = 1;
		}
		if (vendorid == vendorList[1]){
			multiTouchConnectedFlag = 1;
		}
		if (vendorid == vendorList[2]){
			eyeXConnectedFlag = 1;
		}
		if (productid == vendorList[3])
		{
			realSenseConnectedFlag = 1;
		}
	}

	stateCounter = 0;

	stateCounter = stateCounter + (setLeapState());

	stateCounter = stateCounter + (setMultiTouchState());

	stateCounter = stateCounter + (setEyeXState());

	stateCounter = stateCounter + (setRealSenseState());

	resetFlags();

	libusb_free_device_list(devs, 1);


	libusb_exit(NULL);

	return stateCounter;
}

Mode::DefaultModes DeviceHandler::getDefaultMode(){

	if (multiTouchConnected && leapConnected && eyeXConnected && realSenseConnected){
		return set_MLER_Mode();
	}
	else if (multiTouchConnected && leapConnected && eyeXConnected){ //&& eyeXConnectedFlag){
		return set_MLE_Mode();
	}
	else if (multiTouchConnected && leapConnected && realSenseConnected){
		return set_MLR_Mode();
	}
	else if (multiTouchConnected && eyeXConnected && realSenseConnected){
		return set_MER_Mode();
	}
	else if (leapConnected && eyeXConnected && realSenseConnected){
		return set_LER_Mode();
	}
	else if (leapConnected && eyeXConnected){
		return set_LE_Mode();
	}
	else if (multiTouchConnected && eyeXConnected){
		return set_ME_Mode();
	}
	else if (multiTouchConnected && leapConnected){
		return set_ML_Mode();
	}
	else if (multiTouchConnected && realSenseConnected){
		return set_MR_Mode();
	}
	else if (leapConnected && realSenseConnected){
		return set_LR_Mode();
	}
	else if (eyeXConnected && realSenseConnected){
		return set_ER_Mode();
	}
	else if (leapConnected){
		return set_L_Mode();
	}
	else if (multiTouchConnected){
		return set_M_Mode();
	}
	else if (realSenseConnected){
		return set_R_Mode();
	}
	else{
		return set_E_Mode();
	}
	
}

int DeviceHandler::leapStatus(){
	return leapConnected;
}

int DeviceHandler::multiTouchStatus(){
	return multiTouchConnected;
}

int DeviceHandler::eyeXStatus(){
	return eyeXConnected;
}

int DeviceHandler::realSenseStatus(){
	return realSenseConnected;
}

int DeviceHandler::setLeapState(){

	if (overrideLeap){
		return 0;
	}

	if (leapConnectedFlag == 1){
		if (leapConnected == 0){
			leapConnected = 1;
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if (leapConnected == 1){
			leapConnected = 0;
			return 1;
		}
		else{
			return 0;
		}
	}
}

int DeviceHandler::setMultiTouchState(){

	if (overrideMultiTouch){
		return 0;
	}

	if( multiTouchConnectedFlag == 1){
		if (multiTouchConnected == 0){

			multiTouchConnected = 1;
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if (multiTouchConnected == 1){
			multiTouchConnected = 0;
			return 1;
		}
		else{
			return 0;
		}
	}
}

int DeviceHandler::setEyeXState(){

	if (overrideEyeX){
		return 0;
	}

	if (eyeXConnectedFlag == 1){
		if (eyeXConnected == 0){

			eyeXConnected = 1;
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if (eyeXConnected == 1){
			eyeXConnected = 0;
			return 1;
		}
		else{
			return 0;
		}
	}
}

int DeviceHandler::setRealSenseState(){

	if (overrideRealSense){
		return 0;
	}

	if (realSenseConnectedFlag == 1){
		if (realSenseConnected == 0){
			realSenseConnected = 1;
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if (realSenseConnected == 1){
			realSenseConnected = 0;
			return 1;
		}
		else{
			return 0;
		}
	}
}

void DeviceHandler::resetFlags(){
	leapConnectedFlag = 0;
	multiTouchConnectedFlag = 0;
	eyeXConnectedFlag = 0;
	realSenseConnectedFlag = 0;
}

Mode::DefaultModes DeviceHandler::set_MLER_Mode(){
	realSenseExpressionsEnabled = true;
	leapGestureEnabled = false;
	leapDrawEnabled = false;
	return Mode::DefaultModes::MLER;
}
Mode::DefaultModes DeviceHandler::set_MLE_Mode(){

	//Turn off leapDraw
	leapDrawEnabled = false;
	return Mode::DefaultModes::MLE;

}
Mode::DefaultModes DeviceHandler::set_MLR_Mode(){
	realSenseExpressionsEnabled = true;
	leapGestureEnabled = false;
	leapDrawEnabled = false;
	
	return Mode::DefaultModes::MLE;
}
Mode::DefaultModes DeviceHandler::set_MER_Mode(){
	realSenseExpressionsEnabled = true;
	leapDrawEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::MER;
}
Mode::DefaultModes DeviceHandler::set_LER_Mode(){
	realSenseExpressionsEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::LER;
}
Mode::DefaultModes DeviceHandler::set_ML_Mode(){

	leapDrawEnabled = false;
	return Mode::DefaultModes::ML;
}
Mode::DefaultModes DeviceHandler::set_ME_Mode(){
	leapDrawEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::ME;
}
Mode::DefaultModes DeviceHandler::set_MR_Mode(){
	realSenseExpressionsEnabled = true;
	leapDrawEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::MR;
}
Mode::DefaultModes DeviceHandler::set_LE_Mode(){
	return Mode::DefaultModes::LE;
}
Mode::DefaultModes DeviceHandler::set_LR_Mode(){
	realSenseExpressionsEnabled = true;
	leapGestureEnabled = false;
	return Mode::DefaultModes::LR;
}
Mode::DefaultModes DeviceHandler::set_ER_Mode(){
	realSenseExpressionsEnabled = true;
	leapGestureEnabled = false;
	leapDrawEnabled = false;
	return Mode::DefaultModes::LR;
}
Mode::DefaultModes DeviceHandler::set_L_Mode(){
	return Mode::DefaultModes::L;
}
Mode::DefaultModes DeviceHandler::set_E_Mode(){
	leapDrawEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::E;
}
Mode::DefaultModes DeviceHandler::set_M_Mode(){

	leapDrawEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::M;
}
Mode::DefaultModes DeviceHandler::set_R_Mode(){
	realSenseExpressionsEnabled = true;
	leapDrawEnabled = false;
	leapGestureEnabled = false;
	return Mode::DefaultModes::R;
}


#endif
