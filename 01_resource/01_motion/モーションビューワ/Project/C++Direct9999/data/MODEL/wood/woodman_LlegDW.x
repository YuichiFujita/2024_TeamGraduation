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
 52;
 -12.25683;-51.40755;21.22947;,
 -21.22947;-51.40755;12.25683;,
 0.00000;-51.40755;-0.00000;,
 -0.00000;-51.40755;24.51369;,
 12.25683;-51.40755;21.22947;,
 21.22947;-51.40755;12.25683;,
 24.51369;-51.40755;-0.00000;,
 21.22947;-51.40755;-12.25683;,
 12.25683;-51.40755;-21.22947;,
 0.00000;-51.40755;-24.51369;,
 -12.25683;-51.40755;-21.22947;,
 -21.22947;-51.40755;-12.25683;,
 -24.51369;-51.40755;0.00000;,
 0.00000;-47.63070;-20.08233;,
 -10.04118;-47.63070;-17.39184;,
 -6.37026;0.00000;-11.03364;,
 0.00000;0.00000;-12.74055;,
 -17.39181;-47.63070;-10.04118;,
 -11.03364;0.00000;-6.37026;,
 -20.08236;-47.63070;0.00000;,
 -12.74055;0.00000;0.00000;,
 -17.39181;-47.63070;10.04118;,
 -11.03364;0.00000;6.37026;,
 -17.39181;-47.63070;10.04118;,
 -10.04118;-47.63070;17.39184;,
 -6.37026;0.00000;11.03364;,
 -11.03364;0.00000;6.37026;,
 -0.00000;-47.63070;20.08233;,
 0.00000;0.00000;12.74055;,
 10.04118;-47.63070;17.39184;,
 6.37029;0.00000;11.03364;,
 17.39181;-47.63070;10.04118;,
 11.03364;0.00000;6.37026;,
 20.08236;-47.63070;-0.00000;,
 12.74055;0.00000;0.00000;,
 17.39181;-47.63070;-10.04118;,
 11.03364;0.00000;-6.37026;,
 10.04118;-47.63070;-17.39184;,
 6.37029;0.00000;-11.03364;,
 0.00000;0.00000;0.00000;,
 -10.04118;-47.63070;17.39184;,
 -17.39181;-47.63070;10.04118;,
 -0.00000;-47.63070;20.08233;,
 10.04118;-47.63070;17.39184;,
 17.39181;-47.63070;10.04118;,
 20.08236;-47.63070;-0.00000;,
 17.39181;-47.63070;-10.04118;,
 10.04118;-47.63070;-17.39184;,
 0.00000;-47.63070;-20.08233;,
 -10.04118;-47.63070;-17.39184;,
 -17.39181;-47.63070;-10.04118;,
 -20.08236;-47.63070;0.00000;;
 
 48;
 3;0,1,2;,
 3;3,0,2;,
 3;4,3,2;,
 3;5,4,2;,
 3;6,5,2;,
 3;7,6,2;,
 3;8,7,2;,
 3;9,8,2;,
 3;10,9,2;,
 3;11,10,2;,
 3;12,11,2;,
 3;1,12,2;,
 4;13,14,15,16;,
 4;14,17,18,15;,
 4;17,19,20,18;,
 4;19,21,22,20;,
 4;23,24,25,26;,
 4;24,27,28,25;,
 4;27,29,30,28;,
 4;29,31,32,30;,
 4;31,33,34,32;,
 4;33,35,36,34;,
 4;35,37,38,36;,
 4;37,13,16,38;,
 3;34,39,32;,
 3;32,39,30;,
 3;39,28,30;,
 4;40,41,1,0;,
 4;42,40,0,3;,
 4;43,42,3,4;,
 4;44,43,4,5;,
 4;45,44,5,6;,
 4;46,45,6,7;,
 4;47,46,7,8;,
 4;48,47,8,9;,
 4;49,48,9,10;,
 4;50,49,10,11;,
 4;51,50,11,12;,
 4;41,51,12,1;,
 3;39,20,28;,
 3;28,20,25;,
 3;20,26,25;,
 3;34,36,39;,
 3;36,38,39;,
 3;38,16,39;,
 3;39,16,20;,
 3;16,15,20;,
 3;15,18,20;;
 
 MeshMaterialList {
  1;
  48;
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
   0.599200;0.401600;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\wood000.jpg";
   }
  }
 }
 MeshNormals {
  38;
  -0.754766;0.490344;0.435765;,
  -0.435764;0.490344;0.754766;,
  -0.000000;0.490345;0.871529;,
  0.435765;0.490344;0.754766;,
  0.754766;0.490344;0.435764;,
  0.871529;0.490345;0.000000;,
  0.754766;0.490344;-0.435764;,
  0.435765;0.490344;-0.754766;,
  -0.000000;0.490345;-0.871529;,
  -0.435764;0.490344;-0.754766;,
  -0.754766;0.490344;-0.435765;,
  -0.871529;0.490345;0.000000;,
  -0.000000;0.152341;-0.988328;,
  -0.494164;0.152341;-0.855917;,
  -0.855917;0.152341;-0.494164;,
  -0.988328;0.152341;0.000000;,
  -0.855917;0.152341;0.494164;,
  -0.494164;0.152341;0.855917;,
  -0.000000;0.152341;0.988328;,
  0.494164;0.152341;0.855917;,
  0.855917;0.152341;0.494163;,
  0.988328;0.152341;0.000000;,
  0.855917;0.152341;-0.494163;,
  0.494164;0.152341;-0.855917;,
  0.000000;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.561762;0.761072;0.324334;,
  -0.324334;0.761072;0.561762;,
  0.000000;0.761073;0.648666;,
  0.324334;0.761072;0.561762;,
  0.561762;0.761072;0.324334;,
  0.648666;0.761073;0.000000;,
  0.561762;0.761072;-0.324334;,
  0.324334;0.761072;-0.561762;,
  0.000000;0.761073;-0.648666;,
  -0.324334;0.761072;-0.561762;,
  -0.561762;0.761072;-0.324334;,
  -0.648666;0.761073;0.000000;;
  48;
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  3;24,24,24;,
  4;8,9,13,12;,
  4;9,10,14,13;,
  4;10,11,15,14;,
  4;11,0,16,15;,
  4;0,1,17,16;,
  4;1,2,18,17;,
  4;2,3,19,18;,
  4;3,4,20,19;,
  4;4,5,21,20;,
  4;5,6,22,21;,
  4;6,7,23,22;,
  4;7,8,12,23;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  4;1,0,26,27;,
  4;2,1,27,28;,
  4;3,2,28,29;,
  4;4,3,29,30;,
  4;5,4,30,31;,
  4;6,5,31,32;,
  4;7,6,32,33;,
  4;8,7,33,34;,
  4;9,8,34,35;,
  4;10,9,35,36;,
  4;11,10,36,37;,
  4;0,11,37,26;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;;
 }
 MeshTextureCoords {
  52;
  0.578120;0.979070;,
  0.635320;0.921880;,
  0.500000;0.850000;,
  0.500000;1.000000;,
  0.421880;0.979070;,
  0.364680;0.921880;,
  0.343750;0.843750;,
  0.364680;0.765620;,
  0.421880;0.708430;,
  0.500000;0.687500;,
  0.578120;0.708430;,
  0.635320;0.765620;,
  0.656250;0.843750;,
  0.541670;0.687500;,
  0.562500;0.687500;,
  0.562500;0.499530;,
  0.541670;0.499530;,
  0.583330;0.687500;,
  0.583330;0.499530;,
  0.604170;0.687500;,
  0.604170;0.499530;,
  0.625000;0.687500;,
  0.625000;0.499530;,
  0.375000;0.687500;,
  0.395830;0.687500;,
  0.395830;0.499530;,
  0.375000;0.499530;,
  0.416670;0.687500;,
  0.416670;0.499530;,
  0.437500;0.687500;,
  0.437500;0.499530;,
  0.458330;0.687500;,
  0.458330;0.499530;,
  0.479170;0.687500;,
  0.479170;0.499530;,
  0.500000;0.687500;,
  0.500000;0.499530;,
  0.520830;0.687500;,
  0.520830;0.499530;,
  0.479170;0.499530;,
  0.578120;0.979070;,
  0.635320;0.921880;,
  0.500000;1.000000;,
  0.421880;0.979070;,
  0.364680;0.921880;,
  0.343750;0.843750;,
  0.364680;0.765620;,
  0.421880;0.708430;,
  0.500000;0.687500;,
  0.578120;0.708430;,
  0.635320;0.765620;,
  0.656250;0.843750;;
 }
}
