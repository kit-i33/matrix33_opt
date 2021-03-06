
#include"libOne.h"
//２次元ベクトル（２行１列の行列として考えることもできる）
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};
//３行３列の行列
class MAT33 {
public:
    float _11, _12, _13;
    float _21, _22, _23;
    float _31, _32, _33;
    MAT33(float _11 = 1, float _12 = 0, float _13 = 0,
        float _21 = 0, float _22 = 1, float _23 = 0,
        float _31 = 0, float _32 = 0, float _33 = 1) {
        this->_11 = _11; this->_12 = _12; this->_13 = _13;
        this->_21 = _21; this->_22 = _22; this->_23 = _23;
        this->_31 = _31; this->_32 = _32; this->_33 = _33;
    }
    //単位行列をつくる
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //拡大縮小行列をつくる
    void scaling(float x, float y) {
        _11 = x; _12 = 0; _13 = 0;
        _21 = 0; _22 = y; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //回転行列をつくる
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 =-s; _13 = 0;
        _21 = s; _22 = c; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //平行移動行列をつくる
    void translate(float x, float y) {
        _11 = 1; _12 = 0; _13 = x;
        _21 = 0; _22 = 1; _23 = y;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //「３行３列行列」と「２行１列行列（２次元ベクトル）＋１行」の掛け算
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13 * 1,
            _21 * v.x + _22 * v.y + _23 * 1
        );
    }
    //３行３列行列どうしの掛け算
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //１行目
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //２行目
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //３行目
            _31 + m._11 + _32 * m._21 + _33 * m._31,
            _31 + m._12 + _32 * m._22 + _33 * m._32,
            _31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
    //--------------------------------------------------------
    //最適化されたMAT33同士の掛け算
    //--------------------------------------------------------
    void mulTranslate(float x, float y) {
        // |_11 _12 _13|    |1  0  x| 　 |_11  _12  _11x+_12y+_13| 
        // |_21 _22 _23| × |0  1  y| ＝ |_21  _22  _21x+_22y+_23|
        // |  0   0   1|    |0  0  1|    |  0    0              1|
        _13 += _11 * x + _12 * y;
        _23 += _21 * x + _22 * y;
    }
    void mulScaling(float x, float y) {
        // |_11 _12 _13|    |x  0  0| 　 |_11x _12y _13|
        // |_21 _22 _23| × |0  y  0| ＝ |_21x _22y _23|
        // |  0   0   1|    |0  0  1|    |  0    0    1|
        _11 *= x;    _12 *= y;
        _21 *= x;    _22 *= y;
    }
    void mulRotate(float angle) {
        // |_11 _12 _13|    |c -s  0| 　 |_11c+_12s   _11(-s)+_12c   _13|
        // |_21 _22 _23| × |s  c  0| ＝ |_21c+_22s   _21(-s)+_22c   _23|
        // |  0   0   1|    |0  0  1|    |        0             0      1|
        float c = cos(angle);
        float s = sin(angle);
        float tmp;
        //1行目
        tmp = _11 * c + _12 * s;
        _12 = _11 * -s + _12 * c;
        _11 = tmp;
        //2行目
        tmp = _21 * c + _22 * s;
        _22 = _21 * -s + _22 * c;
        _21 = tmp;
    }
};


#define _MAT33_POINTS


#ifdef _MAT33_TEST
void gmain() {
    window(1000, 1000);
    while (notQuit) {
        clear();
        mathAxis(3.1f, 255);
    }
}
#endif


#ifdef _MAT33_POINTS
void gmain() {
    window(1920, 1080, full);
    colorMode(HSV);
    //頂点を用意する
    const int num = 10;
    COLOR c[num];
    VEC op[num];
    float theta = M_2PI / num;
    for (int i = 0; i < num; i++) {
        float radius = 1.0f - 0.4f * (i % 2);
        op[i].x = sin(theta * i) * radius;
        op[i].y = cos(theta * i) * radius;
        c[i] = COLOR(theta * i, 128, 255);
    }

    VEC p[num];
    MAT33 mat;
    float angle = 0;
    axisMode(NODRAW);
    while (notQuit) {
        clear();
        mathAxis(6.0f, 255);

        mat.identity();
        mat.mulRotate(angle);
        mat.mulTranslate(2.0f, 0);
        mat.mulScaling(1.0f, 0.5f);
        mat.mulRotate(angle * 4);
        angle += 0.01f;

        for (int i = 0; i < num; i++) {
            p[i] = mat * op[i];
        }

        for (int i = 0; i < num; i++) {
            stroke(c[i]);
            strokeWeight(20);
            mathPoint(op[i].x, op[i].y);
            mathPoint(p[i].x, p[i].y);
            strokeWeight(10);
            int j = (i + 1) % num;
            mathLine(op[i].x, op[i].y,op[j].x, op[j].y);
            mathLine(p[i].x, p[i].y,p[j].x, p[j].y);
        }
    }
}
#endif