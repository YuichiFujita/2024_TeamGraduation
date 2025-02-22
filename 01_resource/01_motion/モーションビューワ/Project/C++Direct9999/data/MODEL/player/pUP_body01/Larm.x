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
 37;
 10.44214;-3.44003;-3.09200;,
 0.00000;-3.16419;-2.67157;,
 0.00000;3.16417;-2.67157;,
 10.44214;3.44003;-3.09200;,
 -0.48080;3.55674;0.00000;,
 -0.48080;3.55674;-2.61201;,
 -0.48080;0.00001;-2.61201;,
 -0.48080;0.00001;0.00000;,
 0.00000;-3.16419;2.67157;,
 10.44214;-3.44003;3.09200;,
 10.44214;3.44003;3.09200;,
 0.00000;3.16417;2.67157;,
 10.93702;-3.86678;2.83973;,
 10.93702;-3.86678;-2.83972;,
 10.93702;3.86681;-2.83972;,
 10.93702;3.86681;2.83973;,
 10.44214;4.21031;-2.52631;,
 0.00000;3.63782;-2.32373;,
 0.00000;3.63782;2.32373;,
 10.44214;4.21031;2.52632;,
 10.44214;-4.21031;2.52632;,
 0.00000;-3.63780;2.32373;,
 0.00000;-3.63780;-2.32373;,
 10.44214;-4.21031;-2.52631;,
 10.93702;-3.86678;-2.83972;,
 10.93702;3.86681;-2.83972;,
 -0.48080;-3.55672;-2.61201;,
 -0.48080;-3.55672;2.61201;,
 -0.48080;0.00001;2.61201;,
 -0.48080;3.55674;2.61201;,
 10.93702;3.86681;2.83973;,
 -0.48080;3.55674;2.61201;,
 -0.48080;3.55674;0.00000;,
 10.93702;-3.86678;2.83973;,
 -0.48080;-3.55672;0.00000;,
 -0.48080;-3.55672;2.61201;,
 -0.48080;-3.55672;0.00000;;
 
 33;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,0,3,25;,
 4;25,3,2,5;,
 3;1,26,2;,
 3;2,26,5;,
 3;5,26,6;,
 4;26,1,0,24;,
 3;27,8,28;,
 3;28,8,29;,
 3;29,8,11;,
 4;29,11,10,15;,
 4;15,10,9,12;,
 4;12,9,8,27;,
 4;25,16,19,30;,
 4;30,19,18,31;,
 3;5,32,17;,
 3;32,31,17;,
 3;17,31,18;,
 4;5,17,16,25;,
 4;33,20,23,24;,
 4;24,23,22,26;,
 3;26,22,34;,
 3;34,22,35;,
 3;35,22,21;,
 4;35,21,20,33;,
 4;26,36,7,6;,
 4;28,29,4,7;,
 4;36,27,28,7;;
 
 MeshMaterialList {
  1;
  33;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  33;
  0.886071;-0.463550;-0.000000;,
  0.886061;0.463568;0.000000;,
  -0.088710;-0.325942;-0.941218;,
  -1.000000;0.000000;0.000000;,
  -0.073795;-0.436075;0.896880;,
  -0.088710;0.325938;0.941220;,
  0.131310;-0.119947;-0.984058;,
  0.131310;0.119940;-0.984059;,
  -0.081682;0.087258;-0.992831;,
  -0.067556;-0.116594;-0.990879;,
  -0.090083;-0.069686;0.993493;,
  -0.067556;0.116584;0.990881;,
  0.131305;0.119937;0.984060;,
  0.131305;-0.119944;0.984059;,
  0.171230;0.966660;0.190392;,
  0.171230;0.966660;-0.190392;,
  -0.091077;0.979136;0.181651;,
  -0.122383;0.986597;-0.107930;,
  0.171243;-0.966656;0.190400;,
  0.171243;-0.966656;-0.190400;,
  -0.122384;-0.986596;-0.107934;,
  -0.091078;-0.979135;0.181657;,
  -0.122937;0.000000;0.992414;,
  -0.166287;-0.986077;0.000000;,
  0.140625;-0.447831;-0.882990;,
  0.140626;0.447830;-0.882991;,
  -0.073795;0.436076;-0.896879;,
  0.000000;0.000000;0.000000;,
  0.140620;0.447826;0.882994;,
  0.140619;-0.447827;0.882993;,
  -0.166287;0.986077;0.000000;,
  -0.166287;0.986077;0.000000;,
  -0.166287;-0.986077;0.000000;;
  33;
  4;6,9,8,7;,
  4;3,3,3,3;,
  4;10,13,12,11;,
  4;0,0,1,1;,
  4;15,17,16,14;,
  4;18,21,20,19;,
  4;24,6,7,25;,
  4;25,7,8,26;,
  3;9,2,8;,
  3;8,2,26;,
  3;27,27,27;,
  4;2,9,6,24;,
  3;4,10,22;,
  3;22,10,5;,
  3;5,10,11;,
  4;5,11,12,28;,
  4;28,12,13,29;,
  4;29,13,10,4;,
  4;1,15,14,1;,
  4;28,14,16,5;,
  3;30,31,17;,
  3;31,31,17;,
  3;17,31,16;,
  4;26,17,15,25;,
  4;0,18,19,0;,
  4;24,19,20,2;,
  3;32,20,23;,
  3;23,20,23;,
  3;23,20,21;,
  4;4,21,18,29;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;3,3,3,3;;
 }
 MeshTextureCoords {
  37;
  0.375000;1.000000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.625000;0.625000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.500000;0.625000;,
  0.375000;0.500000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.625000;0.500000;,
  0.375000;0.250000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.875000;0.750000;,
  0.875000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.750000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.375000;0.750000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.750000;0.750000;,
  0.125000;1.000000;,
  0.250000;0.750000;,
  0.125000;0.750000;,
  0.375000;0.625000;;
 }
}
