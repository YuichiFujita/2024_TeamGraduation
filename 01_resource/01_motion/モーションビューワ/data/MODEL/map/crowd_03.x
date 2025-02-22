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
 38;
 -13.35272;46.96021;-13.49389;,
 -13.49389;46.96021;13.35272;,
 -13.49389;73.80718;13.35272;,
 -13.35272;73.80718;-13.49389;,
 -13.35272;73.80718;-13.49389;,
 -13.49389;73.80718;13.35272;,
 13.35272;73.80718;13.49389;,
 13.49389;73.80718;-13.35272;,
 13.49389;73.80718;-13.35272;,
 13.35272;73.80718;13.49389;,
 13.35272;46.96021;13.49389;,
 13.49389;46.96021;-13.35272;,
 13.49389;46.96021;-13.35272;,
 13.35272;46.96021;13.49389;,
 -13.49389;46.96021;13.35272;,
 -13.35272;46.96021;-13.49389;,
 -13.49389;46.96021;13.35272;,
 13.35272;46.96021;13.49389;,
 13.35272;73.80718;13.49389;,
 -13.49389;73.80718;13.35272;,
 13.49389;46.96021;-13.35272;,
 -13.35272;46.96021;-13.49389;,
 -13.35272;73.80718;-13.49389;,
 13.49389;73.80718;-13.35272;,
 -8.01005;0.08975;-8.09473;,
 -8.09473;0.08975;8.01005;,
 -8.09473;46.58473;8.01005;,
 -8.01005;46.58473;-8.09473;,
 8.09473;46.58473;-8.01005;,
 8.01005;46.58473;8.09473;,
 8.01005;0.08975;8.09473;,
 8.09473;0.08975;-8.01005;,
 8.01005;0.08975;8.09473;,
 8.01005;46.58473;8.09473;,
 8.09473;0.08975;-8.01005;,
 -8.01005;0.08975;-8.09473;,
 -8.01005;46.58473;-8.09473;,
 8.09473;46.58473;-8.01005;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;25,32,33,26;,
 4;34,35,36,37;;
 
 MeshMaterialList {
  2;
  10;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\crowd04.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  10;
  -0.999986;0.000000;-0.005258;,
  0.000000;1.000000;0.000000;,
  0.999986;0.000000;0.005258;,
  -0.999986;0.000000;-0.005258;,
  0.999986;0.000000;0.005258;,
  0.000000;-1.000000;0.000000;,
  -0.005258;0.000000;0.999986;,
  0.005258;0.000000;-0.999986;,
  -0.005258;0.000000;0.999986;,
  0.005258;0.000000;-0.999986;;
  10;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;8,8,8,8;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  38;
  0.836590;0.983400;,
  0.991200;0.983400;,
  0.991200;0.828800;,
  0.836590;0.828800;,
  0.467220;0.515480;,
  0.621820;0.515480;,
  0.621820;0.360880;,
  0.467220;0.360880;,
  0.610350;0.828630;,
  0.455750;0.828580;,
  0.455700;0.983180;,
  0.610300;0.983240;,
  0.633940;0.802740;,
  0.788550;0.802740;,
  0.788550;0.648140;,
  0.633940;0.648140;,
  0.664940;0.510550;,
  0.819540;0.510550;,
  0.819540;0.355950;,
  0.664940;0.355950;,
  0.636000;0.983400;,
  0.790600;0.983400;,
  0.790600;0.828800;,
  0.636000;0.828800;,
  0.328510;0.228210;,
  0.505560;0.228210;,
  0.505560;0.051160;,
  0.328510;0.051160;,
  0.721190;0.221400;,
  0.898230;0.221270;,
  0.898110;0.044230;,
  0.721060;0.044350;,
  0.682610;0.228210;,
  0.682610;0.051160;,
  0.102080;0.950080;,
  0.279130;0.950080;,
  0.279130;0.438410;,
  0.102080;0.438410;;
 }
}
