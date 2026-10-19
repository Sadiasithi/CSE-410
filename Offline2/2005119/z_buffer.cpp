#include "MVP_transformation.cpp"
#include "bitmap_image.hpp"
#include <climits>
#include <fstream>
#include <iomanip>

class RGB {
public:
    int r, g, b;
    RGB() : r(0), g(0), b(0) {}
    RGB(int r, int g, int b) : r(r), g(g), b(b) {}
};

static unsigned long int seed = 1;
inline int my_random() {
    seed = (214013 * seed + 2531011);
    return (seed >> 16) & 0x7FFF;
}

RGB drawColor() {
    int r = my_random() % 255;
    int g = my_random() % 255;
    int b = my_random() % 255;
    return RGB(r, g, b);
}

class z_buffer {
public:
    double** zbuffer;
    double screen_width, screen_height;

    double left, right, top, bottom;
    double z_min, z_max;
    double dx, dy, Top_Y, Left_X;

    bitmap_image image;

    z_buffer(double screen_width, double screen_height, double left, double right, double bottom, double top, double z_min, double z_max) {
        this->screen_width = screen_width;
        this->screen_height = screen_height;

        this->left = left;
        this->right = right;
        this->bottom = bottom;
        this->top = top;

        this->z_min = z_min;
        this->z_max = z_max;

        dx = (right - left) / screen_width;
        dy = (top - bottom) / screen_height;

        Top_Y = top - dy / 2;
        Left_X = left + dx / 2;

        zbuffer = new double*[(int)screen_width];
        for (int i = 0; i < screen_width; i++) {
            zbuffer[i] = new double[(int)screen_height];
            for (int j = 0; j < screen_height; j++) {
                zbuffer[i][j] = z_max;
            }
        }

        image.setwidth_height((int)screen_width, (int)screen_height);
    }

    ~z_buffer() {
        for (int i = 0; i < screen_width; i++) {
            delete[] zbuffer[i];
        }
        delete[] zbuffer;
    }

    void procedure() {
        for (auto& t : TriangleSet) {
            RGB c = drawColor();
            double y_max = max3(t.vtx[0].getY(), t.vtx[1].getY(), t.vtx[2].getY());
            double y_min = min3(t.vtx[0].getY(), t.vtx[1].getY(), t.vtx[2].getY());

            int top_scanline = std::max(0, (int)round((Top_Y - y_max) / dy));
            int bottom_scanline = std::min((int)(screen_height - 1), (int)round((Top_Y - y_min) / dy));

            for (int row_no = top_scanline; row_no <= bottom_scanline; row_no++) {
                double scanline_y = Top_Y - row_no * dy;
                double Right_X = right - dx / 2;
                double x_max = INT_MIN, x_min = INT_MAX;
                double z_left = z_max, z_right = z_max;

                for (int i = 0; i < 3; i++) {
                    vector3 a = t.vtx[i];
                    vector3 b = t.vtx[(i + 1) % 3];

                    if (a.getY() == b.getY() && a.getY() == scanline_y) {
                        if (a.getX() < b.getX()) {
                            if (x_max < b.getX()) {
                                x_max = b.getX();
                                z_right = b.getZ();
                            }
                            if (x_min > a.getX()) {
                                x_min = a.getX();
                                z_left = a.getZ();
                            }
                        } 
                        else {
                            if (x_max < a.getX()) {
                                x_max = a.getX();
                                z_right = a.getZ();
                            }
                            if (x_min > b.getX()) {
                                x_min = b.getX();
                                z_left = b.getZ();
                            }
                        }
                    } 
                    else if (a.getY() != b.getY()) {
                        double x = a.getX() + (scanline_y - a.getY()) / slopeXY(a, b);
                        if (x <= std::max(a.getX(), b.getX()) && x >= std::min(a.getX(), b.getX())) {
                            double z = a.getZ() + (scanline_y - a.getY()) * slopeYZ(a, b);
                            if (x < x_min) {
                                x_min = x;
                                z_left = z;
                            }
                            if (x > x_max) {
                                x_max = x;
                                z_right = z;
                            }
                        }
                    }
                }

                if (x_min == INT_MAX || x_max == INT_MIN || x_max == x_min) continue;

                int left_col = std::max(0, (int)round((x_min - Left_X) / dx));
                int right_col = std::min((int)(screen_width - 1), (int)round((x_max - Left_X) / dx));

                for (int col_no = left_col; col_no <= right_col; col_no++) {
                    double scanline_x = Left_X + col_no * dx;
                    double z = z_left + (scanline_x - x_min) * (z_right - z_left) / (x_max - x_min);

                    if (z >= z_min && z <= z_max && z < zbuffer[col_no][row_no]) {
                        zbuffer[col_no][row_no] = z;
                        image.set_pixel(col_no, row_no, c.r, c.g, c.b);
                    }
                }
            }
        }
        image.save_image("output/out.bmp");
    }

    void write_zbuffer() {
        std::ofstream fout("output/z-buffer.txt");
        fout << std::fixed << std::setprecision(6);
    
        for (int j = 0; j < screen_height; j++) {
            bool first = true;
            for (int i = 0; i < screen_width; i++) {
                if (zbuffer[i][j] < z_max) {
                    if (!first) fout;
                    fout << zbuffer[i][j] << "\t";
                    first = false;
                }
            }
            fout << "\n";
        }  
        fout.close();
    }    
};

void z_buffer_pipeline() {
    std::ifstream config("input/config.txt");

    double screen_width, screen_height;
    double left, bottom;
    double z_min, z_max;

    config >> screen_width >> screen_height;
    config >> left;
    config >> bottom;
    
    double right = -left;
    double top = -bottom;

    config >> z_min >> z_max;

    z_buffer zbuffer(screen_width, screen_height, left, right, bottom, top, z_min, z_max);
    zbuffer.procedure();
    zbuffer.write_zbuffer();

    config.close();
}
