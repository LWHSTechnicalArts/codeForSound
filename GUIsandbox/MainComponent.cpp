#include "MainComponent.h"
#include "BinaryData.h"
//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    // load an image that you included in source via Projucer
    myImage = juce::ImageCache::getFromMemory (BinaryData::bunny_jpg,
                                                BinaryData::bunny_jpgSize);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::purple);
    g.setColour (juce::Colours::sandybrown);
    g.drawRect (300, 120, 200, 170, 30); // non-rounded rectangles are straightforward...
    
    // ...while rounded rectangles require the path pattern
    juce::Path path;
    path.addRoundedRectangle(10, 10, 30, 60, 8);
    g.fillPath(path);
    
    // add a sun
    g.setColour (juce::Colours::yellow);
    drawCircle(g, 530, 10, 30, 3.0f);
    
    // triangles require path
    g.setColour (juce::Colours::red);
    juce::Path roof;
    roof.addTriangle (300, 110, 500, 110, 400, 70);
    g.fillPath (roof);
    
    // draw the bunny you loaded into myImage up there in the constructor, with scale and rotate
    const int xDest = 200;
    const int yDest = 100;
    auto transform = juce::AffineTransform::scale (0.05f, 0.05f)
        .translated (xDest, yDest)
        .rotated (
                  juce::MathConstants<float>::pi / 2.0f,
                  xDest,
                  yDest);  // rotate around center
    g.drawImageTransformed(myImage, transform);
    
    g.setFont (juce::FontOptions ("Futura", "regular", 32.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Learning JUCE", getLocalBounds(), juce::Justification::centred, true);
    g.drawText ("Wheee", 20, 40, 60, 40, juce::Justification::centred, true);
    const int numLines = 20;
    const int lineSpacing = getHeight() / numLines;
    
    g.setColour (juce::Colours::green);
    const float myDashLength[] = { 5, 5 };
    for (int i = 0; i < numLines; i++) {
        int y = 5.0f + (i * lineSpacing);
        juce::Line<float> line (0.0f, y, 590.0f, y);
        g.drawDashedLine(line, myDashLength, 2);
        }
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::drawCircle(juce::Graphics& g, float x, float y, float radius, int strokeWidth)
{
    g.drawEllipse(x - radius, y - radius, radius * 2, radius * 2, strokeWidth);
    
}
