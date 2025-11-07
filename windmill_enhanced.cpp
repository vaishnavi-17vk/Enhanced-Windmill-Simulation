

/**
 * ====================================================================
 * ENHANCED WINDMILL SIMULATION - COMPLETE OOP C++ PROJECT
 * ====================================================================
 * 
 * ADVANCED FEATURES:
 * ✅ Multiple independent windmills
 * ✅ Moving clouds animation  
 * ✅ Day/Night mode toggle
 * ✅ Dynamic speed control
 * ✅ Sun/Moon animation
 * ✅ Complete OOP design
 * 
 * OOP CONCEPTS DEMONSTRATED:
 * 1. Encapsulation - Private members
 * 2. Inheritance - Base Drawable class
 * 3. Polymorphism - Virtual functions
 * 4. Abstraction - Abstract base class
 * 5. Composition - Scene contains objects
 * 6. Constructor/Destructor
 * 7. Static members
 * 
 * CONTROLS:
 * - '1', '2', '3' : Select windmill (1st, 2nd, 3rd)
 * - '+' / '=' : Increase selected windmill speed
 * - '-' : Decrease selected windmill speed
 * - 'd' : Day mode
 * - 'n' : Night mode
 * - 'c' : Add new cloud
 * - 'w' : Add new windmill
 * - 's' : Toggle sun/moon animation
 * - 'p' : Pause/Resume all
 * - 'r' : Reset simulation
 * - 'q' / ESC : Exit
 */

#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;



// Window settings
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

// Mode settings
bool isDay = true;
bool isPaused = false;
bool animateCelestial = true;

// Colors
struct Color {
    float r, g, b;
    Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f) 
        : r(red), g(green), b(blue) {}
};


/**
 * @class Drawable
 * @brief Abstract base class for all drawable objects
 */
class Drawable {
protected:
    float x, y;          // Position 
    bool visible;        // Visibility state
    
public:
    // Constructor
    Drawable(float posX = 0.0f, float posY = 0.0f) 
        : x(posX), y(posY), visible(true) {}
    
    // Virtual destructor
    virtual ~Drawable() {}
    
    // Pure virtual functions
    virtual void draw() = 0;
    virtual void update() = 0;
    
    // Getters and setters
    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }
    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }
};


void drawCircle(float cx, float cy, float radius, int segments = 100) {
    glBegin(GL_POLYGON);
    for(int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.14159f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}



/**
 * @class Cloud
 * @brief Moving cloud with animation
 */
class Cloud : public Drawable {
private:
    float speed;         // Private member 
    float size;
    
public:
    Cloud(float posX, float posY, float spd = 0.3f, float sz = 25.0f)
        : Drawable(posX, posY), speed(spd), size(sz) {}
    
    void draw() override {  // Override virtual function
        if(!visible) return;
        
        glColor3f(1.0f, 1.0f, 1.0f);  // White
        
       
        drawCircle(x, y, size);
        drawCircle(x + size * 0.8f, y + size * 0.3f, size * 0.9f);
        drawCircle(x - size * 0.8f, y + size * 0.3f, size * 0.7f);
        drawCircle(x + size * 0.4f, y - size * 0.2f, size * 0.6f);
        drawCircle(x - size * 0.4f, y - size * 0.2f, size * 0.6f);
    }
    
    void update() override {  // Override virtual function 
        if(isPaused) return;
        
        x += speed;
        
        // Wrap around screen
        if(x > 450.0f) {
            x = -450.0f;
            y = randomFloat(150.0f, 280.0f);
        }
    }
    
    // Getter
    float getSpeed() const { return speed; }
    void setSpeed(float s) { speed = s; }
};


/**
 * @class CelestialBody
 * @brief Sun or Moon with animation
 */
class CelestialBody : public Drawable {
private:
    float radius;
    float angle;
    Color color;
    
public:
    CelestialBody(float posX, float posY, float rad, Color col)
        : Drawable(posX, posY), radius(rad), angle(0), color(col) {}
    
    void draw() override {
        if(!visible) return;
        
        glColor3f(color.r, color.g, color.b);
        
        // Draw rays
        if(isDay) {
            for(int i = 0; i < 12; i++) {
                float rayAngle = (360.0f / 12.0f) * i * 3.14159f / 180.0f;
                float x1 = x + (radius + 5) * cos(rayAngle);
                float y1 = y + (radius + 5) * sin(rayAngle);
                float x2 = x + (radius + 15) * cos(rayAngle);
                float y2 = y + (radius + 15) * sin(rayAngle);
                
                glBegin(GL_LINES);
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
                glEnd();
            }
        }
        
        // Draw body
        drawCircle(x, y, radius);
    }
    
    void update() override {
        if(isPaused || !animateCelestial) return;
        
        angle += 0.3f;
        if(angle >= 360.0f) angle = 0.0f;
    }
};


/**
 * @class Windmill
 * @brief Complete windmill with rotating blades
 */
class Windmill : public Drawable {
private:
    // Private members 
    float bladeAngle;
    float rotationSpeed;
    bool isRotating;
    float towerWidth;
    float towerHeight;
    float bladeLength;
    int numBlades;
    int id;  
    
    // Static member 
    static int windmillCount;
    static const int SELECTED_NONE = -1;
    
public:
    static int selectedWindmill;  
    
    // Constructor
    Windmill(float posX, float posY, float tWidth = 30.0f, 
             float tHeight = 120.0f, float bLength = 80.0f, int blades = 4)
        : Drawable(posX, posY),
          bladeAngle(0.0f),
          rotationSpeed(2.0f),
          isRotating(true),
          towerWidth(tWidth),
          towerHeight(tHeight),
          bladeLength(bLength),
          numBlades(blades) {
        
        windmillCount++;
        id = windmillCount;
    }
    
    // Destructor
    ~Windmill() {
        windmillCount--;
    }
    
private:
    void drawTower() {
        // Tower color
        glColor3f(0.55f, 0.27f, 0.07f);  // Brown
        
        // Tower trapezoid
        glBegin(GL_POLYGON);
        glVertex2f(x - towerWidth/2, y);
        glVertex2f(x + towerWidth/2, y);
        glVertex2f(x + towerWidth/3, y + towerHeight);
        glVertex2f(x - towerWidth/3, y + towerHeight);
        glEnd();
        
        // Door
        glColor3f(0.3f, 0.15f, 0.05f);
        glBegin(GL_POLYGON);
        glVertex2f(x - 8, y);
        glVertex2f(x + 8, y);
        glVertex2f(x + 8, y + 30);
        glVertex2f(x - 8, y + 30);
        glEnd();
    }
    
    void drawBlades() {
        float centerX = x;
        float centerY = y + towerHeight;
        
        glPushMatrix();
        glTranslatef(centerX, centerY, 0.0f);
        glRotatef(bladeAngle, 0.0f, 0.0f, 1.0f);
        
        float angleStep = 360.0f / numBlades;
        for(int i = 0; i < numBlades; i++) {
            glPushMatrix();
            glRotatef(i * angleStep, 0.0f, 0.0f, 1.0f);
            
            // Draw blade
            glColor3f(0.95f, 0.95f, 0.90f);  // Off-white
            glBegin(GL_POLYGON);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(-5.0f, bladeLength * 0.3f);
            glVertex2f(-3.0f, bladeLength);
            glVertex2f(3.0f, bladeLength);
            glVertex2f(5.0f, bladeLength * 0.3f);
            glEnd();
            
            // Blade outline
            glColor3f(0.7f, 0.7f, 0.65f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(-5.0f, bladeLength * 0.3f);
            glVertex2f(-3.0f, bladeLength);
            glVertex2f(3.0f, bladeLength);
            glVertex2f(5.0f, bladeLength * 0.3f);
            glEnd();
            
            glPopMatrix();
        }
        
        glPopMatrix();
    }
    
    void drawHub() {
        float centerX = x;
        float centerY = y + towerHeight;
        
        // Hub circle
        glColor3f(0.3f, 0.3f, 0.3f);
        drawCircle(centerX, centerY, 15.0f);
        
        // Center bolt
        glColor3f(0.2f, 0.2f, 0.2f);
        drawCircle(centerX, centerY, 8.0f);
    }
    
    void drawSelectionIndicator() {
        if(selectedWindmill == id) {
            // Draw selection circle around windmill
            glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
            glLineWidth(3.0f);
            
            float centerX = x;
            float centerY = y + towerHeight;
            
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < 50; i++) {
                float angle = 2.0f * 3.14159f * float(i) / 50.0f;
                float px = centerX + 100.0f * cos(angle);
                float py = centerY + 100.0f * sin(angle);
                glVertex2f(px, py);
            }
            glEnd();
            
            glLineWidth(1.0f);
        }
    }
    
public:
    void draw() override {
        if(!visible) return;
        
        drawTower();
        drawBlades();
        drawHub();
        drawSelectionIndicator();
    }
    
    void update() override {
        if(isPaused || !isRotating) return;
        
        bladeAngle += rotationSpeed;
        if(bladeAngle >= 360.0f) bladeAngle -= 360.0f;
    }
    
    // Control methods
    void toggleRotation() { isRotating = !isRotating; }
    void increaseSpeed() { 
        rotationSpeed += 0.5f;
        if(rotationSpeed > 15.0f) rotationSpeed = 15.0f;
    }
    void decreaseSpeed() { 
        rotationSpeed -= 0.5f;
        if(rotationSpeed < 0.5f) rotationSpeed = 0.5f;
    }
    
    // Getters
    bool getIsRotating() const { return isRotating; }
    float getSpeed() const { return rotationSpeed; }
    int getId() const { return id; }
    
    // Static member access
    static int getCount() { return windmillCount; }
};


int Windmill::windmillCount = 0;
int Windmill::selectedWindmill = 1;  // First windmill selected by default

/**
 * @class Scene
 * @brief Manages all objects in the simulation
 */
class Scene {
private:
    vector<Drawable*> objects;  
    vector<Windmill*> windmills;  // Separate windmill reference for control
    vector<Cloud*> clouds;
    CelestialBody* celestialBody;
    
public:
    Scene() {
        celestialBody = nullptr;
    }
    
    ~Scene() {
        // Clean up all dynamically allocated objects
        for(auto obj : objects) {
            delete obj;
        }
        objects.clear();
        windmills.clear();
        clouds.clear();
    }
    
    void addWindmill(Windmill* w) {
        windmills.push_back(w);
        objects.push_back(w);
    }
    
    void addCloud(Cloud* c) {
        clouds.push_back(c);
        objects.push_back(c);
    }
    
    void setCelestialBody(CelestialBody* cb) {
        celestialBody = cb;
        objects.push_back(cb);
    }
    
    void drawAll() {
        for(auto obj : objects) {
            obj->draw();
        }
    }
    
    void updateAll() {
        for(auto obj : objects) {
            obj->update();
        }
    }
    
    vector<Windmill*>& getWindmills() { return windmills; }
    vector<Cloud*>& getClouds() { return clouds; }
    
    void clear() {
        for(auto obj : objects) {
            delete obj;
        }
        objects.clear();
        windmills.clear();
        clouds.clear();
        celestialBody = nullptr;
    }
};


Scene* scene = nullptr;


void drawBackground() {
    // Sky color based on day/night
    if(isDay) {
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);  // Day sky
    } else {
        glClearColor(0.04f, 0.04f, 0.12f, 1.0f);  // Night sky
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Ground
    if(isDay) {
        glColor3f(0.13f, 0.55f, 0.13f);  // Green ground
    } else {
        glColor3f(0.08f, 0.23f, 0.08f);  // Dark green ground
    }
    
    glBegin(GL_POLYGON);
    glVertex2f(-500, -350);
    glVertex2f(500, -350);
    glVertex2f(500, -150);
    glVertex2f(-500, -150);
    glEnd();
}

void drawHUD() {
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Draw text information
    glRasterPos2f(-480, 320);
    string title = "Enhanced Windmill Simulation - OOP Project";
    for(char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    // Mode
    glRasterPos2f(-480, 295);
    string mode = "Mode: ";
    mode += isDay ? "DAY" : "NIGHT";
    mode += isPaused ? " (PAUSED)" : "";
    for(char c : mode) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    // Selected windmill info
    if(!scene->getWindmills().empty() && Windmill::selectedWindmill > 0 
       && Windmill::selectedWindmill <= scene->getWindmills().size()) {
        Windmill* selected = scene->getWindmills()[Windmill::selectedWindmill - 1];
        
        glRasterPos2f(-480, 275);
        char info[100];
        sprintf(info, "Windmill #%d: Speed = %.1f | Status = %s", 
                Windmill::selectedWindmill, 
                selected->getSpeed(),
                selected->getIsRotating() ? "ROTATING" : "STOPPED");
        for(int i = 0; info[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);
        }
    }
    
    // Controls
    int yPos = -320;
    glRasterPos2f(-480, yPos);
    string controls = "Controls: 1/2/3-Select | +/-Speed | D-Day | N-Night | C-Cloud | W-Windmill | S-Sun | P-Pause | R-Reset | Q-Quit";
    for(char c : controls) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}


void display() {
    drawBackground();
    
    if(scene) {
        scene->drawAll();
    }
    
    drawHUD();
    
    glutSwapBuffers();
}

void timer(int value) {
    if(scene) {
        scene->updateAll();
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            {
                int selection = key - '0';
                if(selection > 0 && selection <= scene->getWindmills().size()) {
                    Windmill::selectedWindmill = selection;
                    cout << "Selected Windmill #" << selection << endl;
                }
            }
            break;
            
        case '+':
        case '=':
            if(!scene->getWindmills().empty() && Windmill::selectedWindmill > 0 
               && Windmill::selectedWindmill <= scene->getWindmills().size()) {
                scene->getWindmills()[Windmill::selectedWindmill - 1]->increaseSpeed();
                cout << "Speed increased to: " 
                     << scene->getWindmills()[Windmill::selectedWindmill - 1]->getSpeed() << endl;
            }
            break;
            
        case '-':
        case '_':
            if(!scene->getWindmills().empty() && Windmill::selectedWindmill > 0 
               && Windmill::selectedWindmill <= scene->getWindmills().size()) {
                scene->getWindmills()[Windmill::selectedWindmill - 1]->decreaseSpeed();
                cout << "Speed decreased to: " 
                     << scene->getWindmills()[Windmill::selectedWindmill - 1]->getSpeed() << endl;
            }
            break;
            
        case 'd':
        case 'D':
            isDay = true;
            cout << "Switched to DAY mode" << endl;
            break;
            
        case 'n':
        case 'N':
            isDay = false;
            cout << "Switched to NIGHT mode" << endl;
            break;
            
        case 'c':
        case 'C':
            {
                float cloudX = randomFloat(-450.0f, 450.0f);
                float cloudY = randomFloat(150.0f, 280.0f);
                float cloudSpeed = randomFloat(0.2f, 0.5f);
                scene->addCloud(new Cloud(cloudX, cloudY, cloudSpeed));
                cout << "Added new cloud" << endl;
            }
            break;
            
        case 'w':
        case 'W':
            {
                float windmillX = randomFloat(-400.0f, 400.0f);
                float windmillY = randomFloat(-300.0f, -180.0f);
                scene->addWindmill(new Windmill(windmillX, windmillY));
                cout << "Added Windmill #" << Windmill::getCount() << endl;
            }
            break;
            
        case 's':
        case 'S':
            animateCelestial = !animateCelestial;
            cout << "Sun/Moon animation: " << (animateCelestial ? "ON" : "OFF") << endl;
            break;
            
        case 'p':
        case 'P':
            isPaused = !isPaused;
            cout << "Simulation: " << (isPaused ? "PAUSED" : "RESUMED") << endl;
            break;
            
        case 'r':
        case 'R':
            cout << "Resetting simulation..." << endl;
            scene->clear();
            Windmill::selectedWindmill = 1;
            // Re-initialize will be done by init() function
            break;
            
        case 'q':
        case 'Q':
        case 27:  // ESC
            cout << "Exiting..." << endl;
            delete scene;
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-500, 500, -350, 350);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void initScene() {
    srand(time(NULL));
    
    scene = new Scene();
    
    // Add windmills
    scene->addWindmill(new Windmill(-250.0f, -200.0f, 30.0f, 120.0f, 80.0f, 4));
    scene->addWindmill(new Windmill(100.0f, -220.0f, 35.0f, 130.0f, 90.0f, 4));
    scene->addWindmill(new Windmill(350.0f, -210.0f, 28.0f, 110.0f, 75.0f, 4));
    
    // Add clouds
    scene->addCloud(new Cloud(-300.0f, 220.0f, 0.3f, 25.0f));
    scene->addCloud(new Cloud(0.0f, 250.0f, 0.25f, 30.0f));
    scene->addCloud(new Cloud(250.0f, 200.0f, 0.35f, 28.0f));
    
    // Add sun/moon
    scene->setCelestialBody(new CelestialBody(350.0f, 250.0f, 30.0f, Color(1.0f, 0.95f, 0.0f)));
}

void init() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-500, 500, -350, 350);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    initScene();
}


int main(int argc, char** argv) {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║     ENHANCED WINDMILL SIMULATION - OOP PROJECT        ║\n";
    cout << "║                                                       ║\n";
    cout << "║          OpenGL & FreeGLUT - Advanced Version         ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";
    cout << "\n";
    
    cout << "Features:\n";
    cout << "  ✓ Multiple Independent Windmills\n";
    cout << "  ✓ Moving Clouds\n";
    cout << "  ✓ Day/Night Mode Toggle\n";
    cout << "  ✓ Individual Speed Control\n";
    cout << "  ✓ Dynamic Object Addition\n";
    cout << "  ✓ Complete OOP Design\n";
    cout << "\n";
    
    cout << "Controls:\n";
    cout << "  1/2/3     - Select windmill\n";
    cout << "  +/-       - Adjust speed\n";
    cout << "  D/N       - Day/Night mode\n";
    cout << "  C         - Add cloud\n";
    cout << "  W         - Add windmill\n";
    cout << "  S         - Toggle sun animation\n";
    cout << "  P         - Pause/Resume\n";
    cout << "  R         - Reset\n";
    cout << "  Q/ESC     - Exit\n";
    cout << "\n";
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Enhanced Windmill Simulation - OOP Project");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    
    cout << "Starting simulation...\n" << endl;
    
    glutMainLoop();
    
    return 0;
}

