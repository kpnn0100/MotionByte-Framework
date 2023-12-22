#pragma once
#include <vector>
#include <memory>
namespace pertyG
{
    class Segment
    {
    private:
        std::vector<std::shared_ptr<Segment>> mChildrenList;
    public:

    };
}