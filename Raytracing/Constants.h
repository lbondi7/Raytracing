#pragma once

const int WIDTH = 720;
const int HEIGHT = 405;

#define THREADED_BUILD false
#define THREADED_TRAVERSE true

//const int WIDTH = 600;
//const int HEIGHT = 300;
const float ASPECT_RATIO = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);

const float kEPSILON = 1e-5;