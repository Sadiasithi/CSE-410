
#include<bits/stdc++.h>
using namespace std ; 
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define pi (2 * acos(0.0)) 
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define CYAN 5
#define MAGENTA 6
#define BLACK 7
#define WHITE 8
#define epsilon 0.0000001

class Object;
class PointLight;
class SpotLight;

class Vector3D {
    public:
    double x, y, z; 
     
    Vector3D() {
        x = 0, y = 0, z = 0;
    }
    Vector3D(double a ,double b , double c) {
        x = a, y = b, z = c; 
    }
    double getx(){return x;} 
    double gety(){return y;} 
    double getz(){return z;} 

    void setx(double a){x = a;}
    void sety(double b){y = b;}
    void setz(double c){z = c;}

    Vector3D addition(Vector3D p){
        return Vector3D(x + p.getx(), y + p.gety(), z + p.getz());
    }
    Vector3D subtraction(Vector3D p){
        return Vector3D(x - p.getx(), y - p.gety(), z - p.getz());
    }
    Vector3D scalarMul(double n){
        return Vector3D(x * n, y * n, z * n);
    }
    Vector3D crossMul(Vector3D p){
        return Vector3D(y * p.getz() - z * p.gety(), z * p.getx() - x * p.getz(), x * p.gety() - y * p.getx());
    }
    double dotMul(Vector3D p){
        return x*p.getx() + y*p.gety() + z*p.getz();
    }
    void show(string str = "KEYBOARD"){
        cout << str << " " << x << "," << y << "," << z << "---" << endl; 
    }
    void normalize(){
        double len = sqrt(x*x + y*y + z*z); 
        x = x*1.0/len; 
        y = y*1.0/len; 
        z = z*1.0/len;   
    }
    double distance(Vector3D p){
        return sqrt((x - p.getx())*(x - p.getx())+(y - p.gety())*(y - p.gety())+(z - p.getz())*(z - p.getz())) ; 
    }
}; 

Vector3D RodriGeneral(Vector3D rotatingVector, Vector3D withRespectTo, double angle){
    Vector3D perpendicular = rotatingVector.crossMul(withRespectTo);
    return rotatingVector.scalarMul(cos(angle)).addition(perpendicular.scalarMul(sin(angle)));
}


class Color {
public:
    double r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(double r, double g, double b) : r(r), g(g), b(b) {}

    void setcolor(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    void setcolor(const Color& color) {
        r = color.r;
        g = color.g;
        b = color.b;
    }
    void normalize() {
        r = max(0.0, min(1.0, r));
        g = max(0.0, min(1.0, g));
        b = max(0.0, min(1.0, b));
    }
    Color addition(const Color& c) const {
        Color result(r + c.r, g + c.g, b + c.b);
        result.normalize();
        return result;
    }  
    Color multiplication(double value) const {
        Color result(r * value, g * value, b * value);
        result.normalize();
        return result;
    }
    Color multiplication(const Color& c) const {
        Color result(r * c.r, g * c.g, b * c.b);
        result.normalize();
        return result;
    }
    Color addition(double value) const {
        Color result(r + value, g + value, b + value);
        result.normalize();
        return result;
    }
};

class Texture {
public:
    unsigned char* data;
    int width, height, channels;
    bool loaded;
    
    Texture() : data(nullptr), width(0), height(0), channels(0), loaded(false) {}
    
    Texture(const char* filename) {
        data = stbi_load(filename, &width, &height, &channels, 3);
        loaded = (data != nullptr);
        if (!loaded) {
            cout << "Failed to load texture: " << filename << endl;
        }
    }
    
    ~Texture() {
        if (data) {
            stbi_image_free(data);
        }
    }
    
    Color getColorAt(double u, double v) {
        if (!loaded || !data) {
            return Color(0.5, 0.5, 0.5);
        }
               
        u = max(0.0, min(1.0, u));
        v = max(0.0, min(1.0, v));
        int x = (int)(u * (width - 1));
        int y = (int)((1.0 - v) * (height - 1));
        
        x = max(0, min(width - 1, x));
        y = max(0, min(height - 1, y));
        
        int index = (y * width + x) * 3;
        double r = data[index] / 255.0;
        double g = data[index + 1] / 255.0;
        double b = data[index + 2] / 255.0;
        
        return Color(r, g, b);
    }
};


class Ray{
public:
    Vector3D start, dir;
    Ray(){
        start = Vector3D(0, 0, 0); 
        dir = Vector3D(0, 0, 0);
    }
    Ray(Vector3D a, Vector3D b){
        start = a; 
        dir = b; 
        dir.normalize(); 
    }
};


void drawSphere(double radius, Color color, int slices, int stacks){
    Vector3D SpherePoints[100][100]; 
    double h, r;
    for(int i = 0; i <= stacks; i++){
        h = radius*sin(((double)i / (double)stacks) * (pi/2));
        r = radius*cos(((double)i / (double)stacks) * (pi/2));
        for(int j = 0; j <= slices; j++){
            SpherePoints[i][j].x = r * cos(((double)j/(double)slices) * 2 * pi);
            SpherePoints[i][j].y = r * sin(((double)j/(double)slices) * 2 * pi);
            SpherePoints[i][j].z = h;
        }
    }
    for(int i = 0; i < stacks; i++){
        for(int j = 0; j < slices; j++){
            glBegin(GL_QUADS);{ 
                glColor3f(color.r, color.g, color.b);  
                // front face
                glVertex3f(SpherePoints[i][j].x, SpherePoints[i][j].y, SpherePoints[i][j].z);
                glVertex3f(SpherePoints[i][j+1].x, SpherePoints[i][j+1].y, SpherePoints[i][j+1].z);
                glVertex3f(SpherePoints[i+1][j+1].x, SpherePoints[i+1][j+1].y, SpherePoints[i+1][j+1].z);
                glVertex3f(SpherePoints[i+1][j].x, SpherePoints[i+1][j].y, SpherePoints[i+1][j].z);
                // back face
                glVertex3f(SpherePoints[i][j].x, SpherePoints[i][j].y, -SpherePoints[i][j].z);
                glVertex3f(SpherePoints[i][j+1].x, SpherePoints[i][j+1].y, -SpherePoints[i][j+1].z);
                glVertex3f(SpherePoints[i+1][j+1].x, SpherePoints[i+1][j+1].y, -SpherePoints[i+1][j+1].z);
                glVertex3f(SpherePoints[i+1][j].x, SpherePoints[i+1][j].y, -SpherePoints[i+1][j].z);
            }glEnd();
        }
    }
}

class PointLight{
    public:
    Vector3D light_position;
    Color plColor;
    PointLight() {      
        light_position = Vector3D(0, 0, 0);
    }
    PointLight(Vector3D poisition, Color col){
        this->light_position = poisition; 
        this->plColor = col; 
    }
    friend istream &operator>>(istream &input , PointLight &pl){
        input >> pl.light_position.x >> pl.light_position.y >> pl.light_position.z >> pl.plColor.r >> pl.plColor.g >> pl.plColor.b; 
        return input; 
    }
    void draw(){
        glPushMatrix() ; 
        glTranslatef(light_position.x, light_position.y, light_position.z);
        drawSphere(1, plColor, 30, 30);
        glPopMatrix();
    }
};

class SpotLight {
    public:
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle; 
    SpotLight(){
        point_light = PointLight(); 
        light_direction = Vector3D(0, 0, 0); 
        cutoff_angle = 0; 
    }
    SpotLight(PointLight point_light, Vector3D light_direction, double cutoff_angle) {
        this->point_light = point_light; 
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
        this->light_direction.normalize(); 
    }
    friend istream &operator>>(istream &input, SpotLight &sl){
        input >> sl.point_light >> sl.light_direction.x >> sl.light_direction.y >> sl.light_direction.z >> sl.cutoff_angle; 
        return input; 
    }
    void draw(){
        glPushMatrix() ; 
        glTranslatef(point_light.light_position.x,point_light.light_position.y, point_light.light_position.z) ;
        drawSphere(2, point_light.plColor, 30, 30) ;
        glPopMatrix() ;
    }
};
 
extern int recursion_level;
extern vector<Object*> objects;
extern vector<PointLight> point_lights;
extern vector<SpotLight> spot_lights;
extern bool useFloorTexture;
extern Vector3D position_of_camera;

class Object {
public:
    Vector3D reference_point;
    double height, width, length;
    Color color;
    double ambient, diffuse, specular, reflection_coeff;   
    int shine;
    string type;

    Object() {}
    virtual void draw() = 0;
    void setColor() {}
    void setShine() {}
    void setCoEfficients() {}
    virtual Color getColorAt(Vector3D intersectionPoint) = 0;
    virtual Vector3D getNormal(Vector3D IntersectionPoint) = 0;
    virtual double calculateRayIntersection(Ray &r, string str) = 0;
    
    // Virtual method to determine if shadows should be calculated for this object type
    virtual bool shouldCalculateShadows() const { return false; }

    void reflection_calculation(Vector3D &L, Vector3D &normal, Vector3D &intersectionPoint, Color &color, int level){
        int near = -1; 
        double min = 1000000;
        int count = 0; 
        Color reflectedColor;
        Vector3D R = L.subtraction(normal.scalarMul(2*L.dotMul(normal)));
        R.normalize(); 
       
        Vector3D start = intersectionPoint.addition(R.scalarMul(epsilon));
        Ray reflectedRay(start,R);
        for(Object *o : objects){
            double t = o->intersect(reflectedRay, reflectedColor, 0);
            if(t > 0 && t < min){
                min = t;
                near = count;
            }
            count++; 
        }
        if(near != -1){
            min =  objects[near]->intersect(reflectedRay,reflectedColor,level+1);
            color = color.addition(reflectedColor.multiplication(reflection_coeff));
        }
    }

    double intersect(Ray &r, Color &curr_col, int level){
        double min = calculateRayIntersection(r, this->type);
        if(level == 0) return min; 
        
        Vector3D intersectionPoint = r.start.addition(r.dir.scalarMul(min));
        Color IPcolor = getColorAt(intersectionPoint);
        curr_col = IPcolor.multiplication(ambient);
        Vector3D normal = getNormal(intersectionPoint);

        vector<PointLight> all_lights;
        for(int i = 0; i < point_lights.size(); i++){
            all_lights.push_back(point_lights[i]); 
        }   
        for(int i = 0; i < spot_lights.size(); i++){
            Vector3D slDirection = spot_lights[i].light_direction;
            Vector3D slIntersect = intersectionPoint.subtraction(spot_lights[i].point_light.light_position);
            slDirection.normalize(); 
            slIntersect.normalize(); 
            double createdAngle = acos(slDirection.dotMul(slIntersect));
            if((createdAngle*180/pi) <=spot_lights[i].cutoff_angle) all_lights.push_back(spot_lights[i].point_light);        
        }
      
        for(int i = 0; i < all_lights.size(); i++){
            Vector3D L = all_lights[i].light_position.subtraction(intersectionPoint);
            Vector3D N = normal;  
            Vector3D R = N.scalarMul(2*(L.dotMul(N))).subtraction(L);
            Vector3D V = position_of_camera.subtraction(intersectionPoint);
            
            double distanceFromIntersectionToLight = intersectionPoint.distance(all_lights[i].light_position);
            
            L.normalize();
            V.normalize();
            
            // Create shadow ray from intersection point toward light (offset slightly to avoid self-intersection)
            Vector3D shadowRayStart = intersectionPoint.addition(L.scalarMul(epsilon));
            Ray shadowRay(shadowRayStart, L);
            Ray reflectedRay(intersectionPoint, R);

            double LdotN = L.dotMul(N);
            double RdotV = reflectedRay.dir.dotMul(r.dir);
           
            bool isBlocked = false;
            
            // Only calculate shadows for objects that should have shadows (Floor)
            if(shouldCalculateShadows()) {
                double t2, min_t = 1000000 ;
                for(Object *o : objects){
                    if(o != this) {
                        Color tempColor;
                        t2 = o->intersect(shadowRay, tempColor, 0); 
                        if(t2 > 0 && t2 < min_t) min_t = t2;
                    }
                }
                if(min_t > 0 && min_t < distanceFromIntersectionToLight-epsilon) isBlocked = true;
            }
            
            if(!isBlocked){
                double lambert_value = max(0.0, LdotN);
                double phong_value = max(0.0, pow(RdotV, shine)) ;
                Color temp_color = all_lights[i].plColor.multiplication(getColorAt(intersectionPoint)) ; 
                Color lambert_Component = temp_color.multiplication(lambert_value * diffuse) ;
                temp_color = all_lights[i].plColor.multiplication(getColorAt(intersectionPoint)) ;
                Color phong_Component = temp_color.multiplication(phong_value * specular) ;
                curr_col = curr_col.addition(lambert_Component) ;
                curr_col = curr_col.addition(phong_Component) ;
                curr_col.normalize() ;
            }
        }

        if(level >= recursion_level) return min;
        reflection_calculation(r.dir, normal , intersectionPoint , curr_col , level) ;
        return min ; 
    }
};

struct Floor: public Object{
    Vector3D initial_point; 
    double tilewidth;
    double floorwidth;
    Texture* texture;
    
    Floor(){
        initial_point = Vector3D(0, 0, 0); 
        floorwidth = 0; 
        texture = nullptr;
    }
    Floor(Vector3D initial_point, double floorwidth, double tilewidth, Color color, double ambient, double diffuse, double specular, double reflection_coeff, int shine){
        this->initial_point = initial_point;
        this->floorwidth = floorwidth;
        this->tilewidth = tilewidth; 
        this->color.setcolor(color);
        this->ambient = ambient; 
        this->diffuse = diffuse; 
        this->specular = specular; 
        this->reflection_coeff = reflection_coeff; 
        this->shine = shine; 
        this->texture = nullptr;
    }
    Floor(Vector3D initial_point, double floorwidth, double tilewidth, Color color, double ambient, double diffuse, double specular, double reflection_coeff, int shine, const char* textureFile){
        this->initial_point = initial_point;
        this->floorwidth = floorwidth;
        this->tilewidth = tilewidth; 
        this->color.setcolor(color);
        this->ambient = ambient; 
        this->diffuse = diffuse; 
        this->specular = specular; 
        this->reflection_coeff = reflection_coeff; 
        this->shine = shine; 
        this->texture = new Texture(textureFile);
    }
    
    ~Floor() {
        if (texture) {
            delete texture;
        }
    }
    
    // Override to enable shadows on floor
    bool shouldCalculateShadows() const override { return true; }
    
    void draw() override{
        int floorRow = (int)(floorwidth/tilewidth);
        int floorCol = (int)(floorwidth/tilewidth); 
        for(int i = 0; i < floorRow; i++){
            for(int j = 0; j < floorCol; j++){
                if((i+j)%2 == 0) glColor3f(color.r, color.g, color.b);
                else{
                    glColor3f(1-color.r, 1-color.g, 1-color.b); 
                }
                glBegin(GL_QUADS); 
                glVertex3f(initial_point.x + i * tilewidth, initial_point.y + j * tilewidth, initial_point.z); 
                glVertex3f(initial_point.x + (i + 1) * tilewidth, initial_point.y + j * tilewidth, initial_point.z); 
                glVertex3f(initial_point.x + (i + 1) * tilewidth, initial_point.y + (j + 1) * tilewidth, initial_point.z); 
                glVertex3f(initial_point.x + i * tilewidth, initial_point.y + (j + 1) * tilewidth, initial_point.z); 
                glEnd(); 
            }
        }
    }
    double calculateRayIntersection(Ray &r, string str) override{
        Vector3D normal(0, 0, 1); 
        double up = -1.0* (r.start.dotMul(normal));
        double down = r.dir.dotMul(normal) ;
        if(down == 0) return -1;
        double t = up/down;
        if(t<0) return -1;
        Vector3D intersectPoint = r.start.addition(r.dir.scalarMul(t));
        if(intersectPoint.x < initial_point.x || intersectPoint.x > -initial_point.x) return -1;
        if(intersectPoint.y < initial_point.y || intersectPoint.y > -initial_point.y) return -1;
        return t;
    }
    Vector3D getNormal(Vector3D intersectionPoint) override{
        Vector3D normal = Vector3D(0, 0, 1); 
        normal.normalize(); 
        return normal; 
    }
    Color getColorAt(Vector3D intersectionPoint) override{
        if (useFloorTexture && texture && texture->loaded) {
            // Calculate which tile we're in
            double relativeX = intersectionPoint.x - initial_point.x;
            double relativeY = intersectionPoint.y - initial_point.y;
            
            // Make coordinates positive by adding floor width/2 (since initial_point is at -floorwidth/2)
            relativeX += floorwidth/2;
            relativeY += floorwidth/2;
            
            // Calculate position within the current tile (0 to tilewidth)
            double tileX = fmod(relativeX, tilewidth);
            double tileY = fmod(relativeY, tilewidth);
            
            // Convert to UV coordinates (0 to 1) within the tile
            double u = tileX / tilewidth;
            double v = tileY / tilewidth;
            
            // Ensure UV coordinates are within bounds
            u = max(0.0, min(1.0, u));
            v = max(0.0, min(1.0, v));
            
            // Get color from texture
            return texture->getColorAt(u, v);
        } 
        else {
            Color col; 
            int r = (int)(abs(intersectionPoint.x - initial_point.x)/tilewidth);
            int c = (int)(abs(intersectionPoint.y - initial_point.y)/tilewidth);
            if((r + c) % 2 == 0) col.setcolor(color);
            else{
                double r = 1 - color.r; 
                double g = 1 - color.g;
                double b = 1 - color.b; 
                col.setcolor(r, g, b);
            }
            return col;
        }
    }
};


struct Sphere : public Object {
public:
    Vector3D center;
    double radius;
    Sphere() {
        center = Vector3D(0, 0, 0);
        radius = 0;
    }
    Sphere(Vector3D center, double radius) {
        this->center = center;
        this->radius = radius;
    }
    Sphere(Vector3D center, double radius, Color col) {
        this->reference_point = center ; 
        this->center = center;
        this->radius = radius;
        this->color.setcolor(col);
    }
    Sphere(Vector3D center, double radius, Color col, double ambient, double diffuse, double specular, double reflection_coeff,  int shine){
        this->reference_point = center; 
        this->center = center;
        this->radius = radius;
        this->color.setcolor(col);
        this->ambient = ambient; 
        this->diffuse = diffuse; 
        this->specular = specular; 
        this->reflection_coeff = reflection_coeff; 
        this->shine = shine; 
    }
    Vector3D getNormal(Vector3D intersectionPoint) override {
        Vector3D normal = intersectionPoint.subtraction(center); 
        normal.normalize(); 
        return normal; 
    }
    Color getColorAt(Vector3D intersectionPoint) override {
        Color col; 
        col.setcolor(color) ;
        return col ;
    }
    void draw(){
       int slices = 30; 
       int stacks = 30; 
       glPushMatrix(); 
       drawSphere(radius, color, slices, stacks); 
       glPopMatrix(); 
    }
    friend istream &operator>>(istream &input, Sphere &s) {
        input >> s.center.x >> s.center.y >> s.center.z >> s.radius >> s.color.r >> s.color.g >> s.color.b >> s.ambient >> s.diffuse >> s.specular >> s.reflection_coeff >> s.shine;
        return input;
    }   
    double calculateRayIntersection(Ray &r , string str) override {       
        Vector3D CurrStart = r.start.subtraction(reference_point); 

        double a = 1;
        double b = (r.dir.dotMul(CurrStart)) * 2;
        double c = (CurrStart.dotMul(CurrStart)) - radius * radius;
        double d = b*b - 4*a*c;
       
        if(d < 0) return -1;
        double t1 = (-b + sqrt(d)) / (2 * a);
        double t2 = (-b - sqrt(d)) / (2 * a);   
        if(t1 > 0 || t2 > 0) {  }
        if(t1 < 0 && t2 < 0) return -1;
        if(t1 < 0) return t2;
        if(t2 < 0) return t1;
        return min(t1, t2);       
    }
};

double determinant(double a[3][3]) {
    double ans = 0;
    for(int i = 0; i < 3; i++){
        ans += a[0][i]*(a[1][(i+1)%3]*a[2][(i+2)%3] - a[1][(i+2)%3]*a[2][(i+1)%3]);
    }
    return ans;
}

struct Triangle : public Object {
    Vector3D point1, point2, point3;
    Triangle(){
        point1 = Vector3D(0, 0, 0);
        point2 = Vector3D(0, 0, 0);
        point3 = Vector3D(0, 0, 0);
    } 
    Triangle(Vector3D point1, Vector3D point2, Vector3D point3) {
        this->point1 = point1;
        this->point2 = point2;
        this->point3 = point3;
    }
    Triangle(Vector3D point1, Vector3D point2, Vector3D point3, Color color, double ambient, double diffuse, double specular, double reflection_coeff, int shine) {
        this->reference_point = Vector3D(0, 0, 0);
        this->point1 = point1;
        this->point2 = point2;
        this->point3 = point3;
        this->color.setcolor(color);
        this->ambient = ambient; 
        this->diffuse = diffuse; 
        this->specular = specular; 
        this->reflection_coeff = reflection_coeff; 
        this->shine = shine; 
    }
    friend istream &operator>>(istream &input, Triangle &t) {
        input >> t.point1.x >> t.point1.y >> t.point1.z >> t.point2.x >> t.point2.y >> t.point2.z >> t.point3.x >> t.point3.y >> t.point3.z 
              >> t.color.r >> t.color.g >> t.color.b >> t.ambient >> t.diffuse >> t.specular >> t.reflection_coeff >> t.shine;
        return input;
    }
    void draw() {
        glBegin(GL_TRIANGLES); 
        glColor3f(color.r, color.g, color.b); 
        glVertex3f(point1.x, point1.y, point1.z); 
        glVertex3f(point2.x, point2.y, point2.z); 
        glVertex3f(point3.x, point3.y, point3.z); 
        glEnd();    
    }  
    double calculateRayIntersection(Ray &r, string str) override {
        double detMatrix[3][3]={
            {point1.x - point2.x, point1.x - point3.x, r.dir.x},
            {point1.y - point2.y, point1.y - point3.y, r.dir.y},
            {point1.z - point2.z, point1.z - point3.z, r.dir.z}
        };
        double betaMatrix[3][3]={
            {point1.x - r.start.x, point1.x - point3.x, r.dir.x},
            {point1.y - r.start.y, point1.y - point3.y, r.dir.y},
            {point1.z - r.start.z, point1.z - point3.z, r.dir.z}
        };
        double gammaMatrix[3][3]={
            {point1.x - point2.x, point1.x - r.start.x, r.dir.x},
            {point1.y - point2.y, point1.y - r.start.y, r.dir.y},
            {point1.z - point2.z, point1.z - r.start.z, r.dir.z}
        };
        double tMatrix[3][3]={
            {point1.x - point2.x, point1.x - point3.x, point1.x - r.start.x},
            {point1.y - point2.y, point1.y - point3.y, point1.y - r.start.y},
            {point1.z - point2.z, point1.z - point3.z, point1.z - r.start.z}
        };
        double det = determinant(detMatrix);
        if(det == 0) return -1;
        double beta, gamma, t; 
        beta = determinant(betaMatrix) / det;
        gamma = determinant(gammaMatrix) / det;
        t = determinant(tMatrix) / det;
        if(beta >= 0 && gamma >= 0 && beta + gamma <= 1 && t > 0) return t;

        return -1;   
    }
    Vector3D getNormal(Vector3D intersectionPoint) override {
        Vector3D normal = (point2.subtraction(point1)).crossMul(point3.subtraction(point1));
        normal.normalize(); 
        return normal; 
    }
    Color getColorAt(Vector3D intersectionPoint) override {    
        Color col; 
        col.setcolor(color);
        return col;
    }  
};


struct GeneralQuadSurface : public Object {
    double a, b, c, d, e, f, g, h, i, j; 
    Vector3D cube_reference_point;
    double length, width, height;
    GeneralQuadSurface(){
        a = b = c = d = e = f = g = h = i = j = 0;
        cube_reference_point = Vector3D(0, 0, 0); 
        length = 0; 
        width = 0; 
        height = 0; 
    }
    GeneralQuadSurface(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j,
                       Vector3D cube_reference_point, double length, double width, double height, Color color,
                       double ambient, double diffuse, double specular, double reflection_coeff, int shine) {
        this->a = a, this->b = b, this->c = c, this->d = d, this->e = e, this->f = f, this->g = g, this->h = h, this->i = i, this->j = j;
        this->cube_reference_point = cube_reference_point; 
        this->length = length; 
        this->width = width; 
        this->height = height; 
        this->color.setcolor(color);
        this->ambient = ambient; 
        this->diffuse = diffuse; 
        this->specular = specular; 
        this->reflection_coeff = reflection_coeff; 
        this->shine = shine; 
    }

    friend istream &operator>>(istream &input, GeneralQuadSurface &genQuad) {
        input >> genQuad.a >> genQuad.b >> genQuad.c >> genQuad.d >> genQuad.e >> genQuad.f >> genQuad.g >> genQuad.h >> genQuad.i >> genQuad.j 
              >> genQuad.cube_reference_point.x >> genQuad.cube_reference_point.y >> genQuad.cube_reference_point.z 
              >> genQuad.length >> genQuad.width >> genQuad.height >> genQuad.color.r >> genQuad.color.g >> genQuad.color.b 
              >> genQuad.ambient >> genQuad.diffuse >> genQuad.specular >> genQuad.reflection_coeff >> genQuad.shine; 
        return input; 
    }
    void draw()override{}

    bool isinRange(Vector3D vec) {
        if(length != 0 && (vec.x < cube_reference_point.x || vec.x > cube_reference_point.x + length)) return false;
        if(width != 0 && (vec.y < cube_reference_point.y || vec.y > cube_reference_point.y + width)) return false;
        if(height != 0 && (vec.z < cube_reference_point.z || vec.z > cube_reference_point.z + height)) return false;
        return true; 
    }
    double calculateRayIntersection(Ray &r, string str) override {
        double a_coeff = this->a * r.dir.x * r.dir.x +
                        this->b * r.dir.y * r.dir.y +
                        this->c * r.dir.z * r.dir.z +
                        this->d * r.dir.x * r.dir.y +
                        this->e * r.dir.y * r.dir.z +
                        this->f * r.dir.x * r.dir.z;
        double b_coeff = 2 * this->a * r.start.x * r.dir.x +
                        2 * this->b * r.start.y * r.dir.y +
                        2 * this->c * r.start.z * r.dir.z +
                        this->d * (r.start.x * r.dir.y + r.start.y * r.dir.x) +
                        this->e * (r.start.y * r.dir.z + r.start.z * r.dir.y) +
                        this->f * (r.start.x * r.dir.z + r.start.z * r.dir.x) +
                        this->g * r.dir.x +
                        this->h * r.dir.y +
                        this->i * r.dir.z;
        double c_coeff = this->a * r.start.x * r.start.x +
                        this->b * r.start.y * r.start.y +
                        this->c * r.start.z * r.start.z +
                        this->d * r.start.x * r.start.y +
                        this->e * r.start.y * r.start.z +
                        this->f * r.start.x * r.start.z +
                        this->g * r.start.x +
                        this->h * r.start.y +
                        this->i * r.start.z +
                        this->j;
        double discriminant = b_coeff * b_coeff - 4 * a_coeff * c_coeff;
        if (discriminant < 0) return -1;

        double sqrt_d = sqrt(discriminant);
        double t1 = (-b_coeff + sqrt_d) / (2 * a_coeff);
        double t2 = (-b_coeff - sqrt_d) / (2 * a_coeff);
        if (t1 < 0 && t2 < 0) return -1;

        Vector3D intersectionPoint1 = r.start.addition(r.dir.scalarMul(t1));
        Vector3D intersectionPoint2 = r.start.addition(r.dir.scalarMul(t2));
        if (t1 > 0 && isinRange(intersectionPoint1)) return t1;
        if (t2 > 0 && isinRange(intersectionPoint2)) return t2;
        return -1;
    }
    Vector3D getNormal(Vector3D intersectionPoint) override{
        double dx = 2 * a * intersectionPoint.x + d * intersectionPoint.y + f * intersectionPoint.z + g;
        double dy = 2 * b * intersectionPoint.y + d * intersectionPoint.x + e * intersectionPoint.z + h;
        double dz = 2 * c * intersectionPoint.z + e * intersectionPoint.y + f * intersectionPoint.x + i;
        Vector3D normal = Vector3D(dx, dy, dz); 
        normal.normalize(); 
        return normal; 
    }
    Color getColorAt(Vector3D intersectionPoint) override{
        Color col; 
        col.setcolor(color);
        return col;
    }
};