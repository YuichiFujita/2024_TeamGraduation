xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 10;
 0.00000;33.51549;-7.28509;,
 -50.00000;33.51549;-1.43674;,
 -50.00000;93.72946;10.21491;,
 0.00000;93.72946;-1.43674;,
 50.00000;93.72946;10.21491;,
 50.00000;33.51549;-1.43674;,
 50.00000;-26.69852;6.08010;,
 0.00000;-36.27055;-1.43674;,
 -50.00000;-26.69852;6.08010;,
 0.00000;33.51549;-1.43674;;
 
 8;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;0,5,6,7;,
 4;0,7,8,1;,
 4;3,2,1,9;,
 4;3,9,5,4;,
 4;7,6,5,9;,
 4;7,9,1,8;;
 
 MeshMaterialList {
  2;
  8;
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kite01.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kite00.jpg";
   }
  }
 }
 MeshNormals {
  18;
  -0.169767;0.140970;-0.975349;,
  0.169767;0.140970;-0.975349;,
  -0.139499;-0.102769;-0.984875;,
  0.139499;-0.102769;-0.984875;,
  -0.155814;0.019247;-0.987599;,
  0.155814;0.019247;-0.987599;,
  0.000000;0.019485;-0.999810;,
  0.000000;0.143047;-0.989716;,
  0.000000;-0.103783;-0.994600;,
  0.000000;-0.015810;0.999875;,
  0.000000;-0.094208;0.995552;,
  0.112730;-0.093608;0.989206;,
  0.094010;-0.015740;0.995447;,
  -0.094010;-0.015740;0.995447;,
  -0.112730;-0.093608;0.989206;,
  0.000000;0.062403;0.998051;,
  -0.074684;0.062229;0.995264;,
  0.074684;0.062229;0.995264;;
  8;
  4;6,4,0,7;,
  4;6,7,1,5;,
  4;6,5,3,8;,
  4;6,8,2,4;,
  4;10,11,12,9;,
  4;10,9,13,14;,
  4;15,16,13,9;,
  4;15,9,12,17;;
 }
 MeshTextureCoords {
  10;
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.500000;;
 }
}
