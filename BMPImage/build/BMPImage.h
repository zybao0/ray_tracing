#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <string>
#include <stdexcept>

#ifndef NDEBUG
#include <typeinfo>
#include <cstring>

#endif

/** ------------------数据类型重定义------------------------ */
typedef int int4;              // 4 bytes
typedef unsigned int uint;     // 4 bytes
typedef unsigned char uchar;   // 1 byte
typedef unsigned int uint4;   // 4 bytes
typedef unsigned short uint2;  // 2 bytes
/** -------------------------------------------------------- */

/** ------------------颜色结构体重定义---------------------- */
// Color Palette的结构体，也是RGBA颜色结构体
typedef struct {
    uchar b;
    uchar g;
    uchar r;
    uchar a;
}BGRA,ColorPalette;

// RGB颜色结构体
typedef struct {
    uchar b;
    uchar g;
    uchar r;
}BGR;
/** -------------------------------------------------------- */

class BMPImage {
private:
/** -------------BMP文件头和BMP信息头数据结构定义----------- */
//参考链接如下
    //https://blog.csdn.net/u012877472/article/details/50272771
    //https://blog.csdn.net/lanbing510/article/details/8176231
    //bmp文件头（bmp file header）：共14字节；
    typedef struct {
        uint2 bfType;           //2字节，文件类型；总应是0x4d42('BM')
        uint4 bfSize;           //4字节，文件大小；字节为单位。
        uint2 bfReserved1;      //2字节，保留，必须设置为0
        uint2 bfReserved2;      //2字节，保留，必须设置为0
        uint4 bfOffBits;        //4字节的偏移，表示从文件头到位图数据的偏移
    } BMPFileHeader;

    //位图信息头（bitmap information）：共40字节；
    typedef struct {
        uint4 biSize;           //4字节，信息头的大小，即40；
        int4 biWidth;           //4字节，以像素为单位说明图像的宽度；
        int4 biHeight;          //4字节，以像素为单位说明图像的高度；如果为正，说明位图倒立
        uint2 biPlanes;         //2字节，为目标设备说明颜色平面数，总被设置为1；
        uint2 biBitCount;       //2字节，说明比特数/像素数，值有1、2、4、8、16、24、32；
        uint4 biCompression;    //4字节，说明图像的压缩类型，0(BI_RGB)表示不压缩；
        uint4 biSizeImage;      //4字节，说明位图数据的大小.
        int4 biXPelsPerMeter;   //4字节，表示水平分辨率，单位是像素/米；
        int4 biYPelsPerMeter;   //4字节，表示垂直分辨率，单位是像素/米；
        uint4 biClrUsed;        //4字节，位图使用的调色板中的颜色索引数，为0说明使用所有；
        uint4 biClrImportant;   //4字节，对图像显示有重要影响的颜色索引数，为0说明都重要；
    } BMPInfoHeader;
/** -------------------------------------------------------- */

/** ---------------------公有函数区------------------------- */
public:
    /**
     * 构造函数，初始化一个空的BMPImage
     */
    explicit BMPImage();
    /**
     * 构造函数，载入一张BMP图像,打开失败时会抛出异常
     * @param bmp_file_path 图像路径
     */
    explicit BMPImage(const char* bmp_file_path);
    /**
     * 构造函数，创建大小为width*height的24位的黑色图像，
     * 创建失败时会抛出异常。
     * @param width 宽度
     * @param height 高度
     * @param depth 深度，目前仅支持24位和32位图像
     */
    explicit BMPImage(int4 width, int4 height, uint2 depth = 24);
    /**
     * 拷贝构造函数
     * @param image 图像
     */
    BMPImage(const BMPImage &image);
    /**
     * 赋值构造函数
     * @param image 等号右边的图
     * @return 返回复制的图
     */
    BMPImage &operator=(const BMPImage &image);
    /**
     * 析构函数
     */
    virtual ~BMPImage();
    /**
      * 创建大小为width*height的24位的黑色图像
      * @param width 宽度
      * @param height 高度
      * @param depth 深度，目前仅支持24位图像
      * @return 成功返回true，否则false
      */
    bool create(int4 width,int4 height,uint2 depth=24);
    /**
     * 载入一张BMP图像
     * @param bmp_file_path 图像路径
     * @return 成功返回true，否则false
     */
    bool open(const char* bmp_file_path);
    /**
     * 保存BMP图像
     * @param save_path 保存路径
     * @return 成功返回true，否则false
     */
    bool save(const char* save_path);
    /**
     * 返回(row,col)处像素值的引用
     * @tparam T 可取的值分别为uchar(8位，单通道图像)、
     * BGR(24位，3通道图像)、BGRA(32位，四通道图像)
     * @param row 行
     * @param col 列
     * @return T类型的引用
     */
    template <typename T>
    T & at(uint row, uint col);
    /**
     * 清空图像
     * @return 成功返回true，否则false
     */
    bool clear();

    /**
     * 返回图像宽度
     * @return 宽度
     */
    int4 width();
    /**
     * 返回图像高度
     * @return  高度
     */
    int4 height();
    /**
     * 返回图像深度
     * @return 深度
     */
    uint2 depth();
/** -------------------------------------------------------- */

/** ----------------------私有变量区------------------------ */
private:
    int4 m_width;       //图像宽度
    int4 m_height;      //图像深度
    uint2 m_depth;      //图像深度
    uint4 m_row_size;   //每行字节数
    uchar *m_data;      //图像数据存储区

    BMPFileHeader m_file_header;    //BMP图像的文件头
    BMPInfoHeader m_info_header;    //BMP图像的信息头
    uint m_color_palette_size;      //BMP的调色板数组大小
    ColorPalette *m_color_palette;  //调色板(Color Palette),可选；
/** -------------------------------------------------------- */

/** ------------------------私有函数区------------------------ */
    /**
     * 检查是否越界,以及深度是否匹配
     * @param row 行
     * @param col 列
     * @param depth 深度
     * @return 越界打印错误信息并返回false
     */
     bool valid(int4 row,int4 col,uint2 depth);
/** -------------------------------------------------------- */
};

// 模板函数的实现要放在头文件里
template<typename T>
T &BMPImage::at(uint row, uint col) {
    size_t elem_size = sizeof(T);
#ifdef DEBUG //定义DEBUG时检查
    if(!valid(row,col, (uint2)(elem_size*8))){
        throw std::runtime_error("Index is overflow or T is not match depth");
    }
#endif
    T *value = nullptr;
    // 计算索引
    size_t index = 0;
    if (m_height > 0){
        // 图像数据的存储顺序是从左下到右上，图片上的第一行存在m_data的最后一部分
        index = (m_height-row-1) * m_row_size + col*elem_size;
    }else{
        // 图像数据的存储顺序是从左下到右上，图片上的第一行存在m_data的第一部分
        index = row * m_row_size + col * elem_size;
    }
    value = (T*)(m_data+index);
    return *value;
}

#endif //BMPIMAGE_H
