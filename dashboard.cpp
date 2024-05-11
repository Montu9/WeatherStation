#include "dashboard.h"

#include <ESPAsyncWebServer.h>

static const char index_html[] PROGMEM = R"=="==(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta http-equiv="X-UA-Compatible" content="ie=edge" />
    <title>Evergreen</title>

    <script src="https://cdn.tailwindcss.com"></script>
  </head>
  <body class="bg-indigo-900 w-full h-full">
    <div class="bg-indigo-900 w-full h-full">
      <div class="mx-auto max-w-2xl px-4 py-16 sm:px-6 sm:py-24 lg:px-8">
        <h2 class="text-5xl font-bold tracking-tight text-white">
          Local Climate State
        </h2>

        <div
          class="mt-6 grid grid-cols-1 gap-x-6 gap-y-10 sm:grid-cols-6 relative bg-white px-6 pt-10 pb-8 shadow-xl ring-1 ring-gray-900/5 rounded-lg"
        >
          <!-- Temperature... -->
          <div class="group relative sm:col-span-2">
            <div class="w-full flex justify-center items-center">
              <div
                class="w-12 m-5 aspect-h-1 aspect-w-1 overflow-hidden rounded-md lg:aspect-none group-hover:opacity-75"
              >
                <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
                  <path
                    d="M448 96a32 32 0 1 0 -64 0 32 32 0 1 0 64 0zM320 96a96 96 0 1 1 192 0A96 96 0 1 1 320 96zM144 64c-26.5 0-48 21.5-48 48V276.5c0 17.3-7.1 31.9-15.3 42.5C70.2 332.6 64 349.5 64 368c0 44.2 35.8 80 80 80s80-35.8 80-80c0-18.5-6.2-35.4-16.7-48.9c-8.2-10.6-15.3-25.2-15.3-42.5V112c0-26.5-21.5-48-48-48zM32 112C32 50.2 82.1 0 144 0s112 50.1 112 112V276.5c0 .1 .1 .3 .2 .6c.2 .6 .8 1.6 1.7 2.8c18.9 24.4 30.1 55 30.1 88.1c0 79.5-64.5 144-144 144S0 447.5 0 368c0-33.2 11.2-63.8 30.1-88.1c.9-1.2 1.5-2.2 1.7-2.8c.1-.3 .2-.5 .2-.6V112zM192 368c0 26.5-21.5 48-48 48s-48-21.5-48-48c0-20.9 13.4-38.7 32-45.3V272c0-8.8 7.2-16 16-16s16 7.2 16 16v50.7c18.6 6.6 32 24.4 32 45.3z"
                  />
                </svg>
              </div>
            </div>

            <div class="mt-4 flex justify-between">
              <div>
                <h3 class="text-sm text-gray-700">
                  <a href="#">
                    <span aria-hidden="true" class="absolute inset-0"></span>
                    Temperature
                  </a>
                </h3>
                <p class="mt-1 text-sm text-gray-500">Air</p>
              </div>
              <p class="text-xl font-bold text-gray-900">
              %TEMPERATURE_VAL%
              C</p>
            </div>
          </div>

          <!-- Humidity... -->
          <div class="group relative sm:col-span-2">
            <div class="w-full flex justify-center items-center">
              <div
                class="w-12 m-5 aspect-h-1 aspect-w-1 overflow-hidden rounded-md lg:aspect-none group-hover:opacity-75"
              >
                <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 576 512">
                  <!--!Font Awesome Free 6.5.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2024 Fonticons, Inc.-->
                  <path
                    d="M269.5 69.9c11.1-7.9 25.9-7.9 37 0C329 85.4 356.5 96 384 96c26.9 0 55.4-10.8 77.4-26.1l0 0c11.9-8.5 28.1-7.8 39.2 1.7c14.4 11.9 32.5 21 50.6 25.2c17.2 4 27.9 21.2 23.9 38.4s-21.2 27.9-38.4 23.9c-24.5-5.7-44.9-16.5-58.2-25C449.5 149.7 417 160 384 160c-31.9 0-60.6-9.9-80.4-18.9c-5.8-2.7-11.1-5.3-15.6-7.7c-4.5 2.4-9.7 5.1-15.6 7.7c-19.8 9-48.5 18.9-80.4 18.9c-33 0-65.5-10.3-94.5-25.8c-13.4 8.4-33.7 19.3-58.2 25c-17.2 4-34.4-6.7-38.4-23.9s6.7-34.4 23.9-38.4C42.8 92.6 61 83.5 75.3 71.6c11.1-9.5 27.3-10.1 39.2-1.7l0 0C136.7 85.2 165.1 96 192 96c27.5 0 55-10.6 77.5-26.1zm37 288C329 373.4 356.5 384 384 384c26.9 0 55.4-10.8 77.4-26.1l0 0c11.9-8.5 28.1-7.8 39.2 1.7c14.4 11.9 32.5 21 50.6 25.2c17.2 4 27.9 21.2 23.9 38.4s-21.2 27.9-38.4 23.9c-24.5-5.7-44.9-16.5-58.2-25C449.5 437.7 417 448 384 448c-31.9 0-60.6-9.9-80.4-18.9c-5.8-2.7-11.1-5.3-15.6-7.7c-4.5 2.4-9.7 5.1-15.6 7.7c-19.8 9-48.5 18.9-80.4 18.9c-33 0-65.5-10.3-94.5-25.8c-13.4 8.4-33.7 19.3-58.2 25c-17.2 4-34.4-6.7-38.4-23.9s6.7-34.4 23.9-38.4c18.1-4.2 36.2-13.3 50.6-25.2c11.1-9.4 27.3-10.1 39.2-1.7l0 0C136.7 373.2 165.1 384 192 384c27.5 0 55-10.6 77.5-26.1c11.1-7.9 25.9-7.9 37 0zm0-144C329 229.4 356.5 240 384 240c26.9 0 55.4-10.8 77.4-26.1l0 0c11.9-8.5 28.1-7.8 39.2 1.7c14.4 11.9 32.5 21 50.6 25.2c17.2 4 27.9 21.2 23.9 38.4s-21.2 27.9-38.4 23.9c-24.5-5.7-44.9-16.5-58.2-25C449.5 293.7 417 304 384 304c-31.9 0-60.6-9.9-80.4-18.9c-5.8-2.7-11.1-5.3-15.6-7.7c-4.5 2.4-9.7 5.1-15.6 7.7c-19.8 9-48.5 18.9-80.4 18.9c-33 0-65.5-10.3-94.5-25.8c-13.4 8.4-33.7 19.3-58.2 25c-17.2 4-34.4-6.7-38.4-23.9s6.7-34.4 23.9-38.4c18.1-4.2 36.2-13.3 50.6-25.2c11.1-9.5 27.3-10.1 39.2-1.7l0 0C136.7 229.2 165.1 240 192 240c27.5 0 55-10.6 77.5-26.1c11.1-7.9 25.9-7.9 37 0z"
                  />
                </svg>
              </div>
            </div>

            <div class="mt-4 flex justify-between">
              <div>
                <h3 class="text-sm text-gray-700">
                  <a href="#">
                    <span aria-hidden="true" class="absolute inset-0"></span>
                    Humidity
                  </a>
                </h3>
                <p class="mt-1 text-sm text-gray-500">Air</p>
              </div>
              <p class="text-xl font-bold text-gray-900">
              %HUMIDITY_VAL%
              %</p>
            </div>
          </div>

          <!-- Pressure... -->
          <div class="group relative sm:col-span-2">
            <div class="w-full flex justify-center items-center">
              <div
                class="w-12 m-5 aspect-h-1 aspect-w-1 overflow-hidden rounded-md lg:aspect-none group-hover:opacity-75"
              >
                <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
                  <!--!Font Awesome Free 6.5.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2024 Fonticons, Inc.-->
                  <path
                    d="M0 256a256 256 0 1 1 512 0A256 256 0 1 1 0 256zM288 96a32 32 0 1 0 -64 0 32 32 0 1 0 64 0zM256 416c35.3 0 64-28.7 64-64c0-17.4-6.9-33.1-18.1-44.6L366 161.7c5.3-12.1-.2-26.3-12.3-31.6s-26.3 .2-31.6 12.3L257.9 288c-.6 0-1.3 0-1.9 0c-35.3 0-64 28.7-64 64s28.7 64 64 64zM176 144a32 32 0 1 0 -64 0 32 32 0 1 0 64 0zM96 288a32 32 0 1 0 0-64 32 32 0 1 0 0 64zm352-32a32 32 0 1 0 -64 0 32 32 0 1 0 64 0z"
                  />
                </svg>
              </div>
            </div>

            <div class="mt-4 flex justify-between">
              <div>
                <h3 class="text-sm text-gray-700">
                  <a href="#">
                    <span aria-hidden="true" class="absolute inset-0"></span>
                    Pressure
                  </a>
                </h3>
                <p class="mt-1 text-sm text-gray-500">Air</p>
              </div>
              <p class="text-xl font-bold text-gray-900">
              %PRESSURE_VAL%
              hPa</p>
            </div>
          </div>

          <!-- Brightness... -->
          <div class="group relative sm:col-span-3">
            <div class="w-full flex justify-center items-center">
              <div
                class="w-12 m-5 aspect-h-1 aspect-w-1 overflow-hidden rounded-md lg:aspect-none group-hover:opacity-75"
              >
                <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
                  <!--!Font Awesome Free 6.5.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2024 Fonticons, Inc.-->
                  <path
                    d="M375.7 19.7c-1.5-8-6.9-14.7-14.4-17.8s-16.1-2.2-22.8 2.4L256 61.1 173.5 4.2c-6.7-4.6-15.3-5.5-22.8-2.4s-12.9 9.8-14.4 17.8l-18.1 98.5L19.7 136.3c-8 1.5-14.7 6.9-17.8 14.4s-2.2 16.1 2.4 22.8L61.1 256 4.2 338.5c-4.6 6.7-5.5 15.3-2.4 22.8s9.8 13 17.8 14.4l98.5 18.1 18.1 98.5c1.5 8 6.9 14.7 14.4 17.8s16.1 2.2 22.8-2.4L256 450.9l82.5 56.9c6.7 4.6 15.3 5.5 22.8 2.4s12.9-9.8 14.4-17.8l18.1-98.5 98.5-18.1c8-1.5 14.7-6.9 17.8-14.4s2.2-16.1-2.4-22.8L450.9 256l56.9-82.5c4.6-6.7 5.5-15.3 2.4-22.8s-9.8-12.9-17.8-14.4l-98.5-18.1L375.7 19.7zM269.6 110l65.6-45.2 14.4 78.3c1.8 9.8 9.5 17.5 19.3 19.3l78.3 14.4L402 242.4c-5.7 8.2-5.7 19 0 27.2l45.2 65.6-78.3 14.4c-9.8 1.8-17.5 9.5-19.3 19.3l-14.4 78.3L269.6 402c-8.2-5.7-19-5.7-27.2 0l-65.6 45.2-14.4-78.3c-1.8-9.8-9.5-17.5-19.3-19.3L64.8 335.2 110 269.6c5.7-8.2 5.7-19 0-27.2L64.8 176.8l78.3-14.4c9.8-1.8 17.5-9.5 19.3-19.3l14.4-78.3L242.4 110c8.2 5.7 19 5.7 27.2 0zM256 368a112 112 0 1 0 0-224 112 112 0 1 0 0 224zM192 256a64 64 0 1 1 128 0 64 64 0 1 1 -128 0z"
                  />
                </svg>
              </div>
            </div>

            <div class="mt-4 flex justify-between">
              <div>
                <h3 class="text-sm text-gray-700">
                  <a href="#">
                    <span aria-hidden="true" class="absolute inset-0"></span>
                    Brightness
                  </a>
                </h3>
                <p class="mt-1 text-sm text-gray-500">Air</p>
              </div>
              <p class="text-xl font-bold text-gray-900">
              %BRIGHTNESS_VAL%
              <span>&#37;</span></p>
            </div>
          </div>

          <!-- Moisture... -->
          <div class="group relative sm:col-span-3">
            <div class="w-full flex justify-center items-center">
              <div
                class="w-12 m-5 aspect-h-1 aspect-w-1 overflow-hidden rounded-md lg:aspect-none group-hover:opacity-75"
              >
                <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 384 512">
                  <path
                    d="M192 512C86 512 0 426 0 320C0 228.8 130.2 57.7 166.6 11.7C172.6 4.2 181.5 0 191.1 0h1.8c9.6 0 18.5 4.2 24.5 11.7C253.8 57.7 384 228.8 384 320c0 106-86 192-192 192zM96 336c0-8.8-7.2-16-16-16s-16 7.2-16 16c0 61.9 50.1 112 112 112c8.8 0 16-7.2 16-16s-7.2-16-16-16c-44.2 0-80-35.8-80-80z"
                  />
                </svg>
              </div>
            </div>

            <div class="mt-4 flex justify-between">
              <div>
                <h3 class="text-sm text-gray-700">
                  <a href="#">
                    <span aria-hidden="true" class="absolute inset-0"></span>
                    Moisture
                  </a>
                </h3>
                <p class="mt-1 text-sm text-gray-500">Soil</p>
              </div>
              <p class="text-xl font-bold text-gray-900">
              %MOISTURE_VAL%
              <span>&#37;</span></p>
            </div>
          </div>
        </div>
        <div
          class="space-x-10 snap overflow-x-auto mt-6 flex relative bg-white px-10 pt-10 pb-8 shadow-xl ring-1 ring-gray-900/5 rounded-lg"
        >
          <div class="snap-center">
            <iframe
              width="450"
              height="260"
              style="border: 1px solid #cccccc"
              src="https://thingspeak.com/channels/2543536/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperature&type=line&yaxis=%5BC%5D+Temp"
            ></iframe>
          </div>
          <div class="snap-center">
            <iframe
              width="450"
              height="260"
              style="border: 1px solid #cccccc"
              src="https://thingspeak.com/channels/2543536/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Humidity&type=line&yaxis=%5B%25%5D+Humidity"
            ></iframe>
          </div>
          <div class="snap-center">
            <iframe
              width="450"
              height="260"
              style="border: 1px solid #cccccc"
              src="https://thingspeak.com/channels/2543536/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Pressure&type=line&yaxis=%5BhPa%5D+Pressure"
            ></iframe>
          </div>
          <div class="snap-center">
            <iframe
              width="450"
              height="260"
              style="border: 1px solid #cccccc"
              src="https://thingspeak.com/channels/2543536/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Brightness&type=line&yaxis=%5B%25%5D+Brightness"
            ></iframe>
          </div>
          <div class="snap-center">
            <iframe
              width="450"
              height="260"
              style="border: 1px solid #cccccc"
              src="https://thingspeak.com/channels/2543536/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Moisture&type=line&yaxis=%5B%25%5D+Moisture"
            ></iframe>
          </div>
        </div>
      </div>
    </div>
  </body>
</html>
)=="==";

String Dashboard::processor(const String& var) {
  SensorsData values = storage->getSensorData();
  if (var == "MOISTURE_VAL") {
    String output(values.soilMoisture.value);
    output += "(";
    output += ratingToString(values.soilMoisture.rating);
    output += ")";
    return output;
  } else if (var == "BRIGHTNESS_VAL") {
    return String(values.brightness);
  } else if (var == "HUMIDITY_VAL") {
    return String(values.humidity);
  } else if (var == "TEMPERATURE_VAL") {
    return String(values.temperature);
  } else if (var == "PRESSURE_VAL") {
    return String(values.pressure);
  }

  return String("");
}

Dashboard::Dashboard(AsyncWebServer& server, Storage* storage)
    : webServer(server), storage(storage) {
  webServer.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html,
                    [this](const String& var) { return this->processor(var); });
  });

  // Start server
  webServer.begin();
}
