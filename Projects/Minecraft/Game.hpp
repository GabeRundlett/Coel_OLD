#pragma once

#include <Coel.hpp>
#include "Chunk.hpp"

namespace Minecraft {
    class Game {
      protected:
        Coel::Window window;
        Chunk::Renderer renderer;
        double time{}, prevTime{}, elapsed{};

      public:
        Game();

        bool shouldRun();
        void update();
        void updateTime();

        void onInit();
        void onUpdate();
    };
} // namespace Minecraft
