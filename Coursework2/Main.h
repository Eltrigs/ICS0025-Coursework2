#pragma once
//Windows.h must be included first????
#include <Windows.h>
#include <thread>
#include <iostream>
#include <string>
#include <exception>
#include <random>
#include <fstream>
#include <sstream>
//To do

#include "stdio.h"
#include "time.h"

using namespace std;

#include "Date.h"
#include "Item.h"
#include "Data.h"
#include "NamedPipeClient.h"
//To do

void PrepareNames();
string CreateRandomName();
