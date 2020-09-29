#include "Core/App.hpp"
#include "Core/Base.hpp"

int main(int argc, char *argv[])
{
    App application(argc, argv);

    //SK_ASSERT(false, "Test Assert");
    return application.exec();
}