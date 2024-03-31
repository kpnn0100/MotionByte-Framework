#pragma once
#include "Transformer.h"
#include "util/geometry/Point.h"
#include "util/property/PropertyHeader.h"
namespace MotionByte
{
    class Rotation : public Transformer
    {
    private:
        Point mCenter;
        Property mDegree;
    public:
        // Transform the provided VertexList
        Point& getCenter() {
            return mCenter;
        }
        Property& getDegree() {
            return mDegree;
        }
        virtual void transform(VertexList& vertexList) override
        {
            for (Vertex & vertex : vertexList.getVertexList()) {

                // Calculate the coordinates relative to the rotation center
                float relX = vertex.x - mCenter.getX();
                float relY = vertex.y - mCenter.getY();

                float radians = toRadians(mDegree);
                float rotatedX = relX * std::cos(radians) - relY * std::sin(radians);
                float rotatedY = relX * std::sin(radians) + relY * std::cos(radians);

                // Update the vertex coordinates
                vertex.x = rotatedX + mCenter.getX();
                vertex.y = rotatedY + mCenter.getY();
            }
        }
    };
    class Translation : public Transformer
    {
    private:
        Point mVector;
    public:
        Point& getVector() {
            return mVector;
        }
        // Transform the provided VertexList
        virtual void transform(VertexList& vertexList) override
        {
            for (Vertex& vertex : vertexList.getVertexList())
            {
                // Apply translation by adding the vector to the vertex coordinates
                vertex.x += mVector.getX();
                vertex.y += mVector.getY();
            }
        }
    };
    class TransformerMachine : public Transformer {
    private:
        Rotation mRotation;
        Translation mTranslation;
        PropertyManager mPropertyManager;
    public:
        enum
        {
            RotationX = 0,
            RotationY,
            RotationDegree,
            TranslationX,
            TranslationY,
            PropertyCount
        };
        TransformerMachine() : mPropertyManager(PropertyCount)
        {
            mPropertyManager.setPropertyPointer(RotationX, &mRotation.getCenter().getX());
            mPropertyManager.setPropertyPointer(RotationY, &mRotation.getCenter().getY());
            mPropertyManager.setPropertyPointer(RotationDegree, &mRotation.getDegree());
            mPropertyManager.setPropertyPointer(TranslationX, &mTranslation.getVector().getX());
            mPropertyManager.setPropertyPointer(TranslationY, &mTranslation.getVector().getY());
        }
        PropertyManager & getTransformerMachinePropertyManager()
        {
            return mPropertyManager;
        }
        // Transform the provided VertexList
        virtual void transform(VertexList& vertexList) override
        {
            mRotation.transform(vertexList);
            mTranslation.transform(vertexList);
        }
    };
}
