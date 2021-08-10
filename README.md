# Enlivengine

![CI](https://github.com/Caerind/Enlivengine/workflows/CI/badge.svg?branch=master)

[https://github.com/Caerind/Enlivengine](https://github.com/Caerind/Enlivengine)

My new engine using C++17, SDL & Bgfx

Not really intended to be very stable yet

Used in :
  -  [LudumDare45 : The Story Of Nothing](https://github.com/Caerind/LudumDare45)
  -  [LudumDare46 : Ultimate Chicken Arena](https://github.com/Caerind/LudumDare46)
  -  [Proto : Some random prototypes and things](https://github.com/Caerind/Proto)
  -  [SpaceGame : Team-Building-Game-Jam](https://github.com/Caerind/SpaceGame)

## Getting started

### Build

Using this command should initialize everything and build common things :
```bash
bash UberScript.sh -a
```

There are few others options availables that ease various tasks, you can check them using :
```bash
bash UberScript.sh -h
```

### Set up your project

You can extend the engine for your game quite easily :
  
- Create a folder for your game
- Add this folder to the main CMakeLists.txt
- Add the build target in the Build() function in the UberScript
- Register your components/systems before the Engine::main()


### Code

For the code itself, you can check the [3DSceneExample](https://github.com/Caerind/Enlivengine/blob/master/Enlivengine/EnlivengineExamples/3DSceneExample.cpp)
