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
 306;
 -23.29256;69.84135;-0.00035;,
 -21.51960;69.84135;8.91320;,
 -20.12822;71.44689;8.33686;,
 -21.78661;71.44689;-0.00035;,
 -21.51960;69.84135;-8.91389;,
 -20.12822;71.44689;-8.33755;,
 -16.47054;69.84135;-16.47044;,
 -15.40552;71.44689;-15.40542;,
 -8.91399;69.84135;-21.51950;,
 -8.33765;71.44689;-20.12812;,
 -0.00045;69.84135;-23.29246;,
 -0.00045;71.44689;-21.78651;,
 8.91310;69.84135;-21.51950;,
 8.33676;71.44689;-20.12812;,
 16.46965;69.84135;-16.47044;,
 15.40463;71.44689;-15.40542;,
 21.51871;69.84135;-8.91389;,
 20.12733;71.44689;-8.33755;,
 23.29167;69.84135;-0.00035;,
 21.78572;71.44689;-0.00035;,
 21.51871;69.84135;8.91320;,
 20.12733;71.44689;8.33686;,
 16.46965;69.84135;16.46975;,
 15.40463;71.44689;15.40473;,
 8.91310;69.84135;21.51881;,
 8.33676;71.44689;20.12743;,
 -0.00045;69.84135;23.29177;,
 -0.00045;71.44689;21.78570;,
 -8.91399;69.84135;21.51881;,
 -8.33765;71.44689;20.12743;,
 -16.47054;69.84135;16.46975;,
 -15.40552;71.44689;15.40473;,
 -21.51960;69.84135;8.91320;,
 -20.12822;71.44689;8.33686;,
 -20.12822;72.41248;8.33686;,
 -21.78661;72.41248;-0.00035;,
 -20.12822;72.41248;-8.33755;,
 -15.40552;72.41248;-15.40542;,
 -8.33765;72.41248;-20.12812;,
 -0.00045;72.41248;-21.78651;,
 8.33676;72.41248;-20.12812;,
 15.40463;72.41248;-15.40542;,
 20.12733;72.41248;-8.33755;,
 21.78572;72.41248;-0.00035;,
 20.12733;72.41248;8.33686;,
 15.40463;72.41248;15.40473;,
 8.33676;72.41248;20.12743;,
 -0.00045;72.41248;21.78570;,
 -8.33765;72.41248;20.12743;,
 -15.40552;72.41248;15.40473;,
 -20.12822;72.41248;8.33686;,
 -20.12822;0.00000;8.33686;,
 -15.40552;0.00000;15.40473;,
 -8.33765;0.00000;20.12743;,
 -0.00045;0.00000;21.78570;,
 8.33676;0.00000;20.12743;,
 15.40463;0.00000;15.40473;,
 20.12733;0.00000;8.33686;,
 21.78572;0.00000;-0.00035;,
 20.12733;0.00000;-8.33755;,
 15.40463;0.00000;-15.40542;,
 8.33676;0.00000;-20.12812;,
 -0.00045;0.00000;-21.78651;,
 -8.33765;0.00000;-20.12812;,
 -15.40552;0.00000;-15.40542;,
 -20.12822;0.00000;-8.33755;,
 -21.78661;0.00000;-0.00035;,
 -20.12822;0.00000;8.33686;,
 -20.12822;145.31740;8.33686;,
 -20.12822;144.11564;8.33686;,
 -15.40552;144.11564;15.40473;,
 -15.40552;145.31740;15.40473;,
 -8.33765;144.11564;20.12743;,
 -8.33765;145.31740;20.12743;,
 -0.00045;144.11564;21.78570;,
 -0.00045;145.31740;21.78570;,
 8.33676;144.11564;20.12743;,
 8.33676;145.31740;20.12743;,
 15.40463;144.11564;15.40473;,
 15.40463;145.31740;15.40473;,
 20.12733;144.11564;8.33686;,
 20.12733;145.31740;8.33686;,
 21.78560;144.11564;-0.00035;,
 21.78560;145.31740;-0.00035;,
 20.12733;144.11564;-8.33755;,
 20.12733;145.31740;-8.33755;,
 15.40463;144.11564;-15.40542;,
 15.40463;145.31740;-15.40542;,
 8.33676;144.11564;-20.12812;,
 8.33676;145.31740;-20.12812;,
 -0.00045;144.11564;-21.78639;,
 -0.00045;145.31740;-21.78639;,
 -8.33765;144.11564;-20.12812;,
 -8.33765;145.31740;-20.12812;,
 -15.40552;144.11564;-15.40542;,
 -15.40552;145.31740;-15.40542;,
 -20.12822;144.11564;-8.33755;,
 -20.12822;145.31740;-8.33755;,
 -21.78649;144.11564;-0.00035;,
 -21.78649;145.31740;-0.00035;,
 -20.12822;144.11564;8.33686;,
 -20.12822;145.31740;8.33686;,
 -22.60105;143.69315;-0.00035;,
 -20.88072;143.69315;8.64852;,
 -20.88072;143.69315;-8.64921;,
 -15.98143;143.69315;-15.98133;,
 -8.64931;143.69315;-20.88062;,
 -0.00045;143.69315;-22.60095;,
 8.64842;143.69315;-20.88062;,
 15.98054;143.69315;-15.98133;,
 20.87983;143.69315;-8.64921;,
 22.60016;143.69315;-0.00035;,
 20.87983;143.69315;8.64852;,
 15.98054;143.69315;15.98064;,
 8.64842;143.69315;20.87993;,
 -0.00045;143.69315;22.60026;,
 -8.64931;143.69315;20.87979;,
 -15.98143;143.69315;15.98064;,
 -20.88072;143.69315;8.64852;,
 -21.78649;130.20138;-0.00035;,
 -20.12822;130.20138;8.33686;,
 -20.12822;130.20138;-8.33755;,
 -15.40552;130.20138;-15.40542;,
 -8.33765;130.20138;-20.12812;,
 -0.00045;130.20138;-21.78639;,
 8.33676;130.20138;-20.12812;,
 15.40463;130.20138;-15.40542;,
 20.12733;130.20138;-8.33755;,
 21.78560;130.20138;-0.00035;,
 20.12733;130.20138;8.33686;,
 15.40463;130.20138;15.40473;,
 8.33676;130.20138;20.12743;,
 -0.00045;130.20138;21.78570;,
 -8.33765;130.20138;20.12743;,
 -15.40552;130.20138;15.40473;,
 -20.12822;130.20138;8.33686;,
 -21.78661;102.98012;-0.00035;,
 -20.12822;102.98012;8.33686;,
 -20.12822;102.98012;-8.33755;,
 -15.40552;102.98012;-15.40542;,
 -8.33765;102.98012;-20.12812;,
 -0.00045;102.98012;-21.78651;,
 8.33676;102.98012;-20.12812;,
 15.40463;102.98012;-15.40542;,
 20.12733;102.98012;-8.33755;,
 21.78572;102.98012;-0.00035;,
 20.12733;102.98012;8.33686;,
 15.40463;102.98012;15.40473;,
 8.33676;102.98012;20.12743;,
 -0.00045;102.98012;21.78570;,
 -8.33765;102.98012;20.12743;,
 -15.40552;102.98012;15.40473;,
 -20.12822;102.98012;8.33686;,
 -15.40552;146.52011;15.40473;,
 -20.12822;146.52011;8.33686;,
 -8.33765;146.52011;20.12743;,
 -0.00045;146.52011;21.78570;,
 8.33676;146.52011;20.12743;,
 15.40463;146.52011;15.40473;,
 20.12733;146.52011;8.33686;,
 21.78560;146.52011;-0.00035;,
 20.12733;146.52011;-8.33755;,
 15.40463;146.52011;-15.40542;,
 8.33676;146.52011;-20.12812;,
 -0.00045;146.52011;-21.78639;,
 -8.33765;146.52011;-20.12812;,
 -15.40552;146.52011;-15.40542;,
 -20.12822;146.52011;-8.33755;,
 -21.78649;146.52011;-0.00035;,
 -20.12822;146.52011;8.33686;,
 -15.29942;164.15269;6.33670;,
 -11.70974;164.15269;11.70894;,
 -0.00045;164.15269;-0.00035;,
 -6.33749;164.15269;15.29862;,
 -0.00045;164.15269;16.55914;,
 6.33660;164.15269;15.29862;,
 11.70884;164.15269;11.70894;,
 15.29852;164.15269;6.33670;,
 16.55904;164.15269;-0.00035;,
 15.29852;164.15269;-6.33739;,
 11.70884;164.15269;-11.70964;,
 6.33660;164.15269;-15.29932;,
 -0.00045;164.15269;-16.55983;,
 -6.33749;164.15269;-15.29932;,
 -11.70974;164.15269;-11.70964;,
 -15.29942;164.15269;-6.33739;,
 -16.55993;164.15269;-0.00035;,
 -20.88072;146.94164;8.64852;,
 -22.60105;146.94164;-0.00035;,
 -20.88072;146.94164;-8.64921;,
 -15.98143;146.94164;-15.98133;,
 -8.64931;146.94164;-20.88062;,
 -0.00045;146.94164;-22.60095;,
 8.64842;146.94164;-20.88062;,
 15.98054;146.94164;-15.98133;,
 20.87983;146.94164;-8.64921;,
 22.60016;146.94164;-0.00035;,
 20.87983;146.94164;8.64852;,
 15.98054;146.94164;15.98064;,
 8.64842;146.94164;20.87993;,
 -0.00045;146.94164;22.60026;,
 -8.64931;146.94164;20.87979;,
 -15.98143;146.94164;15.98064;,
 -20.88072;146.94164;8.64852;,
 -20.12822;160.43286;8.33686;,
 -21.78649;160.43286;-0.00035;,
 -20.12822;160.43286;-8.33755;,
 -15.40552;160.43286;-15.40542;,
 -8.33765;160.43286;-20.12812;,
 -0.00045;160.43286;-21.78639;,
 8.33676;160.43286;-20.12812;,
 15.40463;160.43286;-15.40542;,
 20.12733;160.43286;-8.33755;,
 21.78560;160.43286;-0.00035;,
 20.12733;160.43286;8.33686;,
 15.40463;160.43286;15.40473;,
 8.33676;160.43286;20.12743;,
 -0.00045;160.43286;21.78570;,
 -8.33765;160.43286;20.12743;,
 -15.40552;160.43286;15.40473;,
 -20.12822;160.43286;8.33686;,
 -23.29256;74.98410;-0.00035;,
 -21.78661;73.37853;-0.00035;,
 -20.12822;73.37853;8.33686;,
 -21.51960;74.98410;8.91320;,
 -21.51960;74.98410;-8.91389;,
 -20.12822;73.37853;-8.33755;,
 -16.47054;74.98410;-16.47044;,
 -15.40552;73.37853;-15.40542;,
 -8.91399;74.98410;-21.51950;,
 -8.33765;73.37853;-20.12812;,
 -0.00045;74.98410;-23.29246;,
 -0.00045;73.37853;-21.78651;,
 8.91310;74.98410;-21.51950;,
 8.33676;73.37853;-20.12812;,
 16.46965;74.98410;-16.47044;,
 15.40463;73.37853;-15.40542;,
 21.51871;74.98410;-8.91389;,
 20.12733;73.37853;-8.33755;,
 23.29167;74.98410;-0.00035;,
 21.78572;73.37853;-0.00035;,
 21.51871;74.98410;8.91320;,
 20.12733;73.37853;8.33686;,
 16.46965;74.98410;16.46975;,
 15.40463;73.37853;15.40473;,
 8.91310;74.98410;21.51881;,
 8.33676;73.37853;20.12743;,
 -0.00045;74.98410;23.29177;,
 -0.00045;73.37853;21.78570;,
 -8.91399;74.98410;21.51881;,
 -8.33765;73.37853;20.12743;,
 -16.47054;74.98410;16.46975;,
 -15.40552;73.37853;15.40473;,
 -21.51960;74.98410;8.91320;,
 -20.12822;73.37853;8.33686;,
 -20.12838;223.54815;8.33694;,
 -21.78675;223.54815;-0.00035;,
 -20.12838;223.54815;-8.33763;,
 -15.40566;223.54815;-15.40556;,
 -8.33773;223.54815;-20.12828;,
 -0.00045;223.54815;-21.78665;,
 8.33684;223.54815;-20.12828;,
 15.40477;223.54815;-15.40556;,
 20.12748;223.54815;-8.33763;,
 21.78585;223.54815;-0.00035;,
 20.12748;223.54815;8.33694;,
 15.40477;223.54815;15.40487;,
 8.33684;223.54815;20.12759;,
 -0.00045;223.54815;21.78595;,
 -8.33773;223.54815;20.12759;,
 -15.40566;223.54815;15.40487;,
 -20.12838;223.54815;8.33694;,
 -15.53017;223.54815;6.43230;,
 -15.29942;164.15269;6.33670;,
 -16.55993;164.15269;-0.00035;,
 -16.80969;223.54815;-0.00035;,
 -15.29942;164.15269;-6.33739;,
 -15.53017;223.54815;-6.43299;,
 -11.70974;164.15269;-11.70964;,
 -11.88634;223.54815;-11.88624;,
 -6.33749;164.15269;-15.29932;,
 -6.43309;223.54815;-15.53007;,
 -0.00045;164.15269;-16.55983;,
 -0.00045;223.54815;-16.80959;,
 6.33660;164.15269;-15.29932;,
 6.43220;223.54815;-15.53007;,
 11.70884;164.15269;-11.70964;,
 11.88545;223.54815;-11.88624;,
 15.29852;164.15269;-6.33739;,
 15.52928;223.54815;-6.43299;,
 16.55904;164.15269;-0.00035;,
 16.80880;223.54815;-0.00035;,
 15.29852;164.15269;6.33670;,
 15.52928;223.54815;6.43230;,
 11.70884;164.15269;11.70894;,
 11.88545;223.54815;11.88555;,
 6.33660;164.15269;15.29862;,
 6.43220;223.54815;15.52938;,
 -0.00045;164.15269;16.55914;,
 -0.00045;223.54815;16.80890;,
 -6.33749;164.15269;15.29862;,
 -6.43309;223.54815;15.52938;,
 -11.70974;164.15269;11.70894;,
 -11.88634;223.54815;11.88555;,
 -15.29942;164.15269;6.33670;,
 -15.53017;223.54815;6.43230;;
 
 272;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;18,16,17,19;,
 4;20,18,19,21;,
 4;22,20,21,23;,
 4;24,22,23,25;,
 4;26,24,25,27;,
 4;28,26,27,29;,
 4;30,28,29,31;,
 4;32,30,31,33;,
 4;3,2,34,35;,
 4;5,3,35,36;,
 4;7,5,36,37;,
 4;9,7,37,38;,
 4;11,9,38,39;,
 4;13,11,39,40;,
 4;15,13,40,41;,
 4;17,15,41,42;,
 4;19,17,42,43;,
 4;21,19,43,44;,
 4;23,21,44,45;,
 4;25,23,45,46;,
 4;27,25,46,47;,
 4;29,27,47,48;,
 4;31,29,48,49;,
 4;33,31,49,50;,
 4;51,52,30,32;,
 4;52,53,28,30;,
 4;53,54,26,28;,
 4;54,55,24,26;,
 4;55,56,22,24;,
 4;56,57,20,22;,
 4;57,58,18,20;,
 4;58,59,16,18;,
 4;59,60,14,16;,
 4;60,61,12,14;,
 4;61,62,10,12;,
 4;62,63,8,10;,
 4;63,64,6,8;,
 4;64,65,4,6;,
 4;65,66,0,4;,
 4;66,67,1,0;,
 4;68,69,70,71;,
 4;71,70,72,73;,
 4;73,72,74,75;,
 4;75,74,76,77;,
 4;77,76,78,79;,
 4;79,78,80,81;,
 4;81,80,82,83;,
 4;83,82,84,85;,
 4;85,84,86,87;,
 4;87,86,88,89;,
 4;89,88,90,91;,
 4;91,90,92,93;,
 4;93,92,94,95;,
 4;95,94,96,97;,
 4;97,96,98,99;,
 4;99,98,100,101;,
 4;98,102,103,100;,
 4;96,104,102,98;,
 4;94,105,104,96;,
 4;92,106,105,94;,
 4;90,107,106,92;,
 4;88,108,107,90;,
 4;86,109,108,88;,
 4;84,110,109,86;,
 4;82,111,110,84;,
 4;80,112,111,82;,
 4;78,113,112,80;,
 4;76,114,113,78;,
 4;74,115,114,76;,
 4;72,116,115,74;,
 4;70,117,116,72;,
 4;69,118,117,70;,
 4;102,119,120,103;,
 4;104,121,119,102;,
 4;105,122,121,104;,
 4;106,123,122,105;,
 4;107,124,123,106;,
 4;108,125,124,107;,
 4;109,126,125,108;,
 4;110,127,126,109;,
 4;111,128,127,110;,
 4;112,129,128,111;,
 4;113,130,129,112;,
 4;114,131,130,113;,
 4;115,132,131,114;,
 4;116,133,132,115;,
 4;117,134,133,116;,
 4;118,135,134,117;,
 4;119,136,137,120;,
 4;121,138,136,119;,
 4;122,139,138,121;,
 4;123,140,139,122;,
 4;124,141,140,123;,
 4;125,142,141,124;,
 4;126,143,142,125;,
 4;127,144,143,126;,
 4;128,145,144,127;,
 4;129,146,145,128;,
 4;130,147,146,129;,
 4;131,148,147,130;,
 4;132,149,148,131;,
 4;133,150,149,132;,
 4;134,151,150,133;,
 4;135,152,151,134;,
 4;68,71,153,154;,
 4;71,73,155,153;,
 4;73,75,156,155;,
 4;75,77,157,156;,
 4;77,79,158,157;,
 4;79,81,159,158;,
 4;81,83,160,159;,
 4;83,85,161,160;,
 4;85,87,162,161;,
 4;87,89,163,162;,
 4;89,91,164,163;,
 4;91,93,165,164;,
 4;93,95,166,165;,
 4;95,97,167,166;,
 4;97,99,168,167;,
 4;99,101,169,168;,
 3;170,171,172;,
 3;171,173,172;,
 3;173,174,172;,
 3;174,175,172;,
 3;175,176,172;,
 3;176,177,172;,
 3;177,178,172;,
 3;178,179,172;,
 3;179,180,172;,
 3;180,181,172;,
 3;181,182,172;,
 3;182,183,172;,
 3;183,184,172;,
 3;184,185,172;,
 3;185,186,172;,
 3;186,170,172;,
 4;168,169,187,188;,
 4;167,168,188,189;,
 4;166,167,189,190;,
 4;165,166,190,191;,
 4;164,165,191,192;,
 4;163,164,192,193;,
 4;162,163,193,194;,
 4;161,162,194,195;,
 4;160,161,195,196;,
 4;159,160,196,197;,
 4;158,159,197,198;,
 4;157,158,198,199;,
 4;156,157,199,200;,
 4;155,156,200,201;,
 4;153,155,201,202;,
 4;154,153,202,203;,
 4;188,187,204,205;,
 4;189,188,205,206;,
 4;190,189,206,207;,
 4;191,190,207,208;,
 4;192,191,208,209;,
 4;193,192,209,210;,
 4;194,193,210,211;,
 4;195,194,211,212;,
 4;196,195,212,213;,
 4;197,196,213,214;,
 4;198,197,214,215;,
 4;199,198,215,216;,
 4;200,199,216,217;,
 4;201,200,217,218;,
 4;202,201,218,219;,
 4;203,202,219,220;,
 4;221,222,223,224;,
 4;225,226,222,221;,
 4;227,228,226,225;,
 4;229,230,228,227;,
 4;231,232,230,229;,
 4;233,234,232,231;,
 4;235,236,234,233;,
 4;237,238,236,235;,
 4;239,240,238,237;,
 4;241,242,240,239;,
 4;243,244,242,241;,
 4;245,246,244,243;,
 4;247,248,246,245;,
 4;249,250,248,247;,
 4;251,252,250,249;,
 4;253,254,252,251;,
 4;222,35,34,223;,
 4;226,36,35,222;,
 4;228,37,36,226;,
 4;230,38,37,228;,
 4;232,39,38,230;,
 4;234,40,39,232;,
 4;236,41,40,234;,
 4;238,42,41,236;,
 4;240,43,42,238;,
 4;242,44,43,240;,
 4;244,45,44,242;,
 4;246,46,45,244;,
 4;248,47,46,246;,
 4;250,48,47,248;,
 4;252,49,48,250;,
 4;254,50,49,252;,
 4;152,253,251,151;,
 4;151,251,249,150;,
 4;150,249,247,149;,
 4;149,247,245,148;,
 4;148,245,243,147;,
 4;147,243,241,146;,
 4;146,241,239,145;,
 4;145,239,237,144;,
 4;144,237,235,143;,
 4;143,235,233,142;,
 4;142,233,231,141;,
 4;141,231,229,140;,
 4;140,229,227,139;,
 4;139,227,225,138;,
 4;138,225,221,136;,
 4;136,221,224,137;,
 4;255,256,205,204;,
 4;256,257,206,205;,
 4;257,258,207,206;,
 4;258,259,208,207;,
 4;259,260,209,208;,
 4;260,261,210,209;,
 4;261,262,211,210;,
 4;262,263,212,211;,
 4;263,264,213,212;,
 4;264,265,214,213;,
 4;265,266,215,214;,
 4;266,267,216,215;,
 4;267,268,217,216;,
 4;268,269,218,217;,
 4;269,270,219,218;,
 4;270,271,220,219;,
 4;272,273,274,275;,
 4;272,275,256,255;,
 4;275,274,276,277;,
 4;275,277,257,256;,
 4;277,276,278,279;,
 4;277,279,258,257;,
 4;279,278,280,281;,
 4;279,281,259,258;,
 4;281,280,282,283;,
 4;281,283,260,259;,
 4;283,282,284,285;,
 4;283,285,261,260;,
 4;285,284,286,287;,
 4;285,287,262,261;,
 4;287,286,288,289;,
 4;287,289,263,262;,
 4;289,288,290,291;,
 4;289,291,264,263;,
 4;291,290,292,293;,
 4;291,293,265,264;,
 4;293,292,294,295;,
 4;293,295,266,265;,
 4;295,294,296,297;,
 4;295,297,267,266;,
 4;297,296,298,299;,
 4;297,299,268,267;,
 4;299,298,300,301;,
 4;299,301,269,268;,
 4;301,300,302,303;,
 4;301,303,270,269;,
 4;303,302,304,305;,
 4;303,305,271,270;;
 
 MeshMaterialList {
  3;
  272;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.220000;0.220000;0.220000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\take000.jpg";
   }
  }
  Material {
   0.800000;0.780800;0.376800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  264;
  -0.923665;-0.021559;0.382594;,
  -0.999768;-0.021558;0.000000;,
  -0.923665;-0.021559;-0.382594;,
  -0.706942;-0.021559;-0.706942;,
  -0.382594;-0.021559;-0.923665;,
  0.000000;-0.021558;-0.999768;,
  0.382594;-0.021559;-0.923665;,
  0.706942;-0.021559;-0.706942;,
  0.923665;-0.021559;-0.382594;,
  0.999768;-0.021558;-0.000000;,
  0.923665;-0.021559;0.382594;,
  0.706942;-0.021559;0.706942;,
  0.382591;-0.021559;0.923666;,
  -0.000000;-0.021559;0.999768;,
  -0.382591;-0.021559;0.923666;,
  -0.706943;-0.021559;0.706942;,
  -0.858489;0.369521;0.355600;,
  -0.929226;0.369512;0.000000;,
  -0.858489;0.369521;-0.355600;,
  -0.657057;0.369529;-0.657057;,
  -0.355600;0.369521;-0.858489;,
  0.000000;0.369512;-0.929226;,
  0.355600;0.369521;-0.858489;,
  0.657057;0.369529;-0.657057;,
  0.858489;0.369521;-0.355600;,
  0.929226;0.369512;0.000000;,
  0.858489;0.369521;0.355600;,
  0.657058;0.369528;0.657058;,
  0.355595;0.369525;0.858489;,
  -0.000000;0.369520;0.929223;,
  -0.355595;0.369525;0.858489;,
  -0.657058;0.369528;0.657057;,
  -0.862103;0.359533;0.357093;,
  -0.659822;0.359540;0.659822;,
  -0.357090;0.359537;0.862102;,
  -0.000000;0.359533;0.933132;,
  0.357090;0.359537;0.862102;,
  0.659822;0.359540;0.659822;,
  0.862103;0.359533;0.357093;,
  0.933136;0.359525;0.000000;,
  0.862102;0.359533;-0.357093;,
  0.659822;0.359541;-0.659822;,
  0.357093;0.359533;-0.862102;,
  0.000000;0.359525;-0.933135;,
  -0.357093;0.359533;-0.862102;,
  -0.659822;0.359541;-0.659822;,
  -0.862102;0.359533;-0.357093;,
  -0.933136;0.359525;0.000000;,
  -0.923877;0.000000;0.382689;,
  -1.000000;0.000000;0.000000;,
  -0.923877;0.000000;-0.382689;,
  -0.707107;0.000000;-0.707107;,
  -0.382689;0.000000;-0.923877;,
  0.000000;0.000000;-1.000000;,
  0.382689;0.000000;-0.923877;,
  0.707107;0.000000;-0.707107;,
  0.923877;0.000000;-0.382689;,
  1.000000;0.000000;0.000000;,
  0.923877;0.000000;0.382689;,
  0.707107;0.000000;0.707107;,
  0.382682;0.000000;0.923880;,
  0.000000;0.000000;1.000000;,
  -0.382682;0.000000;0.923880;,
  -0.707107;0.000000;0.707107;,
  -0.923880;0.000000;0.382682;,
  -0.382682;0.000000;0.923880;,
  0.382682;0.000000;0.923880;,
  0.707107;0.000000;0.707107;,
  0.923880;0.000000;0.382682;,
  1.000000;0.000000;0.000000;,
  0.923880;0.000000;-0.382683;,
  0.382682;0.000000;-0.923880;,
  0.000000;0.000000;-1.000000;,
  -0.382682;0.000000;-0.923880;,
  -0.923880;0.000000;-0.382683;,
  -1.000000;0.000000;0.000000;,
  -0.923880;0.000000;0.382682;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.923880;0.000000;0.382682;,
  -0.707107;0.000000;0.707107;,
  -0.425398;0.887686;0.176209;,
  -0.460437;0.887692;0.000000;,
  -0.425398;0.887686;-0.176209;,
  -0.325596;0.887679;-0.325596;,
  -0.176209;0.887686;-0.425398;,
  0.000000;0.887692;-0.460437;,
  0.176209;0.887686;-0.425398;,
  0.325596;0.887679;-0.325596;,
  0.425398;0.887686;-0.176209;,
  0.460437;0.887692;0.000000;,
  0.425398;0.887686;0.176209;,
  0.325596;0.887679;0.325596;,
  0.176209;0.887686;0.425398;,
  -0.000005;0.887685;0.460452;,
  -0.176220;0.887672;0.425424;,
  -0.325603;0.887672;0.325608;,
  -0.923459;-0.030144;0.382511;,
  -0.999546;-0.030145;-0.000000;,
  -0.923459;-0.030144;-0.382511;,
  -0.706785;-0.030144;-0.706785;,
  -0.382511;-0.030144;-0.923459;,
  0.000000;-0.030145;-0.999546;,
  0.382511;-0.030144;-0.923459;,
  0.706785;-0.030144;-0.706785;,
  0.923459;-0.030144;-0.382511;,
  0.999546;-0.030145;-0.000000;,
  0.923459;-0.030144;0.382511;,
  0.706785;-0.030144;0.706785;,
  0.382510;-0.030145;0.923460;,
  -0.000002;-0.030145;0.999546;,
  -0.382510;-0.030143;0.923460;,
  -0.706784;-0.030143;0.706787;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.382682;0.000000;0.923880;,
  -0.382682;0.000000;0.923880;,
  -0.707107;0.000000;0.707107;,
  -0.424642;-0.888110;0.175896;,
  -0.459619;-0.888116;-0.000000;,
  -0.424642;-0.888110;-0.175896;,
  -0.325018;-0.888103;-0.325018;,
  -0.175896;-0.888110;-0.424642;,
  -0.000000;-0.888116;-0.459619;,
  0.175896;-0.888110;-0.424642;,
  0.325018;-0.888103;-0.325018;,
  0.424642;-0.888110;-0.175896;,
  0.459619;-0.888116;-0.000000;,
  0.424642;-0.888110;0.175896;,
  0.325018;-0.888103;0.325018;,
  0.175896;-0.888110;0.424642;,
  -0.000005;-0.888109;0.459634;,
  -0.175906;-0.888096;0.424668;,
  -0.325024;-0.888097;0.325029;,
  -0.923459;0.030144;0.382510;,
  -0.999546;0.030145;0.000000;,
  -0.923459;0.030144;-0.382511;,
  -0.706786;0.030143;-0.706786;,
  -0.382511;0.030144;-0.923459;,
  -0.000000;0.030145;-0.999546;,
  0.382511;0.030144;-0.923459;,
  0.706786;0.030143;-0.706786;,
  0.923459;0.030144;-0.382511;,
  0.999546;0.030145;0.000000;,
  0.923459;0.030144;0.382510;,
  0.706786;0.030143;0.706785;,
  0.382510;0.030144;0.923459;,
  -0.000002;0.030144;0.999546;,
  -0.382511;0.030142;0.923459;,
  -0.706784;0.030142;0.706787;,
  -0.923546;0.026870;0.382546;,
  -0.999639;0.026869;0.000000;,
  -0.923545;0.026870;-0.382546;,
  -0.706851;0.026870;-0.706851;,
  -0.382546;0.026870;-0.923546;,
  0.000000;0.026869;-0.999639;,
  0.382546;0.026870;-0.923546;,
  0.706851;0.026870;-0.706851;,
  0.923546;0.026870;-0.382546;,
  0.999639;0.026869;0.000000;,
  0.923546;0.026870;0.382546;,
  0.706851;0.026870;0.706851;,
  0.382543;0.026870;0.923547;,
  0.000000;0.026869;0.999639;,
  -0.382543;0.026870;0.923547;,
  -0.706851;0.026870;0.706851;,
  -0.858490;-0.369517;0.355600;,
  -0.929227;-0.369509;0.000000;,
  -0.858490;-0.369517;-0.355600;,
  -0.657058;-0.369525;-0.657058;,
  -0.355600;-0.369518;-0.858490;,
  0.000000;-0.369509;-0.929227;,
  0.355600;-0.369517;-0.858490;,
  0.657058;-0.369525;-0.657058;,
  0.858490;-0.369517;-0.355600;,
  0.929227;-0.369509;0.000000;,
  0.858490;-0.369517;0.355600;,
  0.657058;-0.369525;0.657058;,
  0.355595;-0.369522;0.858490;,
  -0.000000;-0.369517;0.929224;,
  -0.355595;-0.369522;0.858490;,
  -0.657058;-0.369525;0.657058;,
  -0.867313;-0.344538;0.359251;,
  -0.663810;-0.344546;0.663810;,
  -0.359248;-0.344543;0.867312;,
  -0.000000;-0.344539;0.938772;,
  0.359248;-0.344543;0.867312;,
  0.663810;-0.344546;0.663810;,
  0.867313;-0.344538;0.359251;,
  0.938775;-0.344531;0.000000;,
  0.867313;-0.344539;-0.359251;,
  0.663810;-0.344546;-0.663810;,
  0.359251;-0.344538;-0.867313;,
  0.000000;-0.344531;-0.938775;,
  -0.359251;-0.344539;-0.867313;,
  -0.663810;-0.344546;-0.663810;,
  -0.867313;-0.344539;-0.359251;,
  -0.938775;-0.344531;0.000000;,
  -0.923879;-0.000003;0.382684;,
  -0.707107;-0.000003;0.707107;,
  -0.382684;-0.000003;0.923879;,
  0.000000;-0.000003;1.000000;,
  0.382684;-0.000003;0.923879;,
  0.707107;-0.000003;0.707107;,
  0.923879;-0.000003;0.382684;,
  1.000000;-0.000003;-0.000000;,
  0.923879;-0.000003;-0.382684;,
  0.707107;-0.000003;-0.707107;,
  0.382684;-0.000003;-0.923879;,
  -0.000000;-0.000003;-1.000000;,
  -0.382684;-0.000003;-0.923879;,
  -0.707107;-0.000003;-0.707107;,
  -0.923879;-0.000003;-0.382684;,
  -1.000000;-0.000003;0.000000;,
  0.923870;0.004205;-0.382684;,
  0.707101;0.004205;-0.707101;,
  0.382684;0.004205;-0.923870;,
  0.000000;0.004205;-0.999991;,
  -0.382684;0.004205;-0.923870;,
  -0.707101;0.004205;-0.707101;,
  -0.923870;0.004205;-0.382684;,
  -0.999991;0.004205;0.000000;,
  -0.923870;0.004205;0.382684;,
  -0.707101;0.004205;0.707101;,
  -0.382684;0.004205;0.923870;,
  0.000000;0.004205;0.999991;,
  0.382684;0.004205;0.923870;,
  0.707101;0.004205;0.707101;,
  0.923870;0.004205;0.382684;,
  0.999991;0.004205;0.000000;,
  -0.998183;-0.060262;-0.000000;,
  -0.922200;-0.060260;0.381989;,
  -0.922200;-0.060260;-0.381989;,
  -0.705822;-0.060258;-0.705822;,
  -0.381989;-0.060260;-0.922200;,
  0.000000;-0.060262;-0.998183;,
  0.381989;-0.060260;-0.922200;,
  0.705822;-0.060258;-0.705822;,
  0.922200;-0.060260;-0.381989;,
  0.998183;-0.060262;-0.000000;,
  0.922200;-0.060260;0.381989;,
  0.705822;-0.060258;0.705822;,
  0.381989;-0.060260;0.922200;,
  -0.000004;-0.060260;0.998183;,
  -0.381990;-0.060256;0.922200;,
  -0.705820;-0.060256;0.705824;,
  -0.998182;0.060265;0.000000;,
  -0.922200;0.060263;0.381989;,
  -0.922200;0.060263;-0.381989;,
  -0.705822;0.060260;-0.705822;,
  -0.381989;0.060263;-0.922200;,
  0.000000;0.060265;-0.998182;,
  0.381989;0.060263;-0.922200;,
  0.705822;0.060260;-0.705822;,
  0.922200;0.060263;-0.381989;,
  0.998182;0.060265;0.000000;,
  0.922200;0.060263;0.381989;,
  0.705822;0.060260;0.705822;,
  0.381989;0.060263;0.922200;,
  -0.000004;0.060262;0.998183;,
  -0.381990;0.060258;0.922200;,
  -0.705820;0.060258;0.705824;;
  272;
  4;47,32,16,17;,
  4;46,47,17,18;,
  4;45,46,18,19;,
  4;44,45,19,20;,
  4;43,44,20,21;,
  4;42,43,21,22;,
  4;41,42,22,23;,
  4;40,41,23,24;,
  4;39,40,24,25;,
  4;38,39,25,26;,
  4;37,38,26,27;,
  4;36,37,27,28;,
  4;35,36,28,29;,
  4;34,35,29,30;,
  4;33,34,30,31;,
  4;32,33,31,16;,
  4;17,16,48,49;,
  4;18,17,49,50;,
  4;19,18,50,51;,
  4;20,19,51,52;,
  4;21,20,52,53;,
  4;22,21,53,54;,
  4;23,22,54,55;,
  4;24,23,55,56;,
  4;25,24,56,57;,
  4;26,25,57,58;,
  4;27,26,58,59;,
  4;28,27,59,60;,
  4;29,28,60,61;,
  4;30,29,61,62;,
  4;31,30,62,63;,
  4;16,31,63,48;,
  4;0,15,33,32;,
  4;15,14,34,33;,
  4;14,13,35,34;,
  4;13,12,36,35;,
  4;12,11,37,36;,
  4;11,10,38,37;,
  4;10,9,39,38;,
  4;9,8,40,39;,
  4;8,7,41,40;,
  4;7,6,42,41;,
  4;6,5,43,42;,
  4;5,4,44,43;,
  4;4,3,45,44;,
  4;3,2,46,45;,
  4;2,1,47,46;,
  4;1,0,32,47;,
  4;64,76,80,63;,
  4;63,80,62,65;,
  4;65,62,61,61;,
  4;61,61,60,66;,
  4;66,60,59,67;,
  4;67,59,79,68;,
  4;68,79,78,69;,
  4;69,78,70,70;,
  4;70,70,55,55;,
  4;55,55,71,71;,
  4;71,71,72,72;,
  4;72,72,73,73;,
  4;73,73,51,51;,
  4;51,51,74,74;,
  4;74,74,77,75;,
  4;75,77,76,64;,
  4;82,82,81,81;,
  4;83,83,82,82;,
  4;84,84,83,83;,
  4;85,85,84,84;,
  4;86,86,85,85;,
  4;87,87,86,86;,
  4;88,88,87,87;,
  4;89,89,88,88;,
  4;90,90,89,89;,
  4;91,91,90,90;,
  4;92,92,91,91;,
  4;93,93,92,92;,
  4;94,94,93,93;,
  4;95,95,94,94;,
  4;96,96,95,95;,
  4;81,81,96,96;,
  4;232,98,97,233;,
  4;234,99,98,232;,
  4;235,100,99,234;,
  4;236,101,100,235;,
  4;237,102,101,236;,
  4;238,103,102,237;,
  4;239,104,103,238;,
  4;240,105,104,239;,
  4;241,106,105,240;,
  4;242,107,106,241;,
  4;243,108,107,242;,
  4;244,109,108,243;,
  4;245,110,109,244;,
  4;246,111,110,245;,
  4;247,112,111,246;,
  4;233,97,112,247;,
  4;98,153,152,97;,
  4;99,154,153,98;,
  4;100,155,154,99;,
  4;101,156,155,100;,
  4;102,157,156,101;,
  4;103,158,157,102;,
  4;104,159,158,103;,
  4;105,160,159,104;,
  4;106,161,160,105;,
  4;107,162,161,106;,
  4;108,163,162,107;,
  4;109,164,163,108;,
  4;110,165,164,109;,
  4;111,166,165,110;,
  4;112,167,166,111;,
  4;97,152,167,112;,
  4;64,63,119,76;,
  4;63,65,118,119;,
  4;65,61,61,118;,
  4;61,66,117,61;,
  4;66,67,116,117;,
  4;67,68,79,116;,
  4;68,69,115,79;,
  4;69,70,70,115;,
  4;70,55,55,70;,
  4;55,71,71,55;,
  4;71,72,72,71;,
  4;72,73,73,72;,
  4;73,51,51,73;,
  4;51,74,74,51;,
  4;74,75,114,74;,
  4;75,64,76,114;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  3;113,113,113;,
  4;121,120,120,121;,
  4;122,121,121,122;,
  4;123,122,122,123;,
  4;124,123,123,124;,
  4;125,124,124,125;,
  4;126,125,125,126;,
  4;127,126,126,127;,
  4;128,127,127,128;,
  4;129,128,128,129;,
  4;130,129,129,130;,
  4;131,130,130,131;,
  4;132,131,131,132;,
  4;133,132,132,133;,
  4;134,133,133,134;,
  4;135,134,134,135;,
  4;120,135,135,120;,
  4;248,249,136,137;,
  4;250,248,137,138;,
  4;251,250,138,139;,
  4;252,251,139,140;,
  4;253,252,140,141;,
  4;254,253,141,142;,
  4;255,254,142,143;,
  4;256,255,143,144;,
  4;257,256,144,145;,
  4;258,257,145,146;,
  4;259,258,146,147;,
  4;260,259,147,148;,
  4;261,260,148,149;,
  4;262,261,149,150;,
  4;263,262,150,151;,
  4;249,263,151,136;,
  4;199,169,168,184;,
  4;198,170,169,199;,
  4;197,171,170,198;,
  4;196,172,171,197;,
  4;195,173,172,196;,
  4;194,174,173,195;,
  4;193,175,174,194;,
  4;192,176,175,193;,
  4;191,177,176,192;,
  4;190,178,177,191;,
  4;189,179,178,190;,
  4;188,180,179,189;,
  4;187,181,180,188;,
  4;186,182,181,187;,
  4;185,183,182,186;,
  4;184,168,183,185;,
  4;169,49,48,168;,
  4;170,50,49,169;,
  4;171,51,50,170;,
  4;172,52,51,171;,
  4;173,53,52,172;,
  4;174,54,53,173;,
  4;175,55,54,174;,
  4;176,56,55,175;,
  4;177,57,56,176;,
  4;178,58,57,177;,
  4;179,59,58,178;,
  4;180,60,59,179;,
  4;181,61,60,180;,
  4;182,62,61,181;,
  4;183,63,62,182;,
  4;168,48,63,183;,
  4;152,184,185,167;,
  4;167,185,186,166;,
  4;166,186,187,165;,
  4;165,187,188,164;,
  4;164,188,189,163;,
  4;163,189,190,162;,
  4;162,190,191,161;,
  4;161,191,192,160;,
  4;160,192,193,159;,
  4;159,193,194,158;,
  4;158,194,195,157;,
  4;157,195,196,156;,
  4;156,196,197,155;,
  4;155,197,198,154;,
  4;154,198,199,153;,
  4;153,199,184,152;,
  4;200,215,137,136;,
  4;215,214,138,137;,
  4;214,213,139,138;,
  4;213,212,140,139;,
  4;212,211,141,140;,
  4;211,210,142,141;,
  4;210,209,143,142;,
  4;209,208,144,143;,
  4;208,207,145,144;,
  4;207,206,146,145;,
  4;206,205,147,146;,
  4;205,204,148,147;,
  4;204,203,149,148;,
  4;203,202,150,149;,
  4;202,201,151,150;,
  4;201,200,136,151;,
  4;216,216,231,231;,
  4;113,113,113,113;,
  4;231,231,230,230;,
  4;113,113,113,113;,
  4;230,230,229,229;,
  4;113,113,113,113;,
  4;229,229,228,228;,
  4;113,113,113,113;,
  4;228,228,227,227;,
  4;113,113,113,113;,
  4;227,227,226,226;,
  4;113,113,113,113;,
  4;226,226,225,225;,
  4;113,113,113,113;,
  4;225,225,224,224;,
  4;113,113,113,113;,
  4;224,224,223,223;,
  4;113,113,113,113;,
  4;223,223,222,222;,
  4;113,113,113,113;,
  4;222,222,221,221;,
  4;113,113,113,113;,
  4;221,221,220,220;,
  4;113,113,113,113;,
  4;220,220,219,219;,
  4;113,113,113,113;,
  4;219,219,218,218;,
  4;113,113,113,113;,
  4;218,218,217,217;,
  4;113,113,113,113;,
  4;217,217,216,216;,
  4;113,113,113,113;;
 }
 MeshTextureCoords {
  306;
  0.609380;0.633440;,
  0.625000;0.633440;,
  0.625000;0.635080;,
  0.609380;0.635080;,
  0.593750;0.633440;,
  0.593750;0.635080;,
  0.578120;0.633440;,
  0.578120;0.635080;,
  0.562500;0.633440;,
  0.562500;0.635080;,
  0.546880;0.633440;,
  0.546880;0.635080;,
  0.531250;0.633440;,
  0.531250;0.635080;,
  0.515620;0.633440;,
  0.515620;0.635080;,
  0.500000;0.633440;,
  0.500000;0.635080;,
  0.484380;0.633440;,
  0.484380;0.635080;,
  0.468750;0.633440;,
  0.468750;0.635080;,
  0.453120;0.633440;,
  0.453120;0.635080;,
  0.437500;0.633440;,
  0.437500;0.635080;,
  0.421880;0.633440;,
  0.421880;0.635080;,
  0.406250;0.633440;,
  0.406250;0.635080;,
  0.390620;0.633440;,
  0.390620;0.635080;,
  0.375000;0.633440;,
  0.375000;0.635080;,
  0.625000;0.637410;,
  0.609380;0.637410;,
  0.593750;0.637410;,
  0.578120;0.637410;,
  0.562500;0.637410;,
  0.546880;0.637410;,
  0.531250;0.637410;,
  0.515620;0.637410;,
  0.500000;0.637410;,
  0.484380;0.637410;,
  0.468750;0.637410;,
  0.453120;0.637410;,
  0.437500;0.637410;,
  0.421880;0.637410;,
  0.406250;0.637410;,
  0.390620;0.637410;,
  0.375000;0.637410;,
  0.375000;0.630410;,
  0.390620;0.630410;,
  0.406250;0.630410;,
  0.421880;0.630410;,
  0.437500;0.630410;,
  0.453120;0.630410;,
  0.468750;0.630410;,
  0.484380;0.630410;,
  0.500000;0.630410;,
  0.515620;0.630410;,
  0.531250;0.630410;,
  0.546880;0.630410;,
  0.562500;0.630410;,
  0.578120;0.630410;,
  0.593750;0.630410;,
  0.609380;0.630410;,
  0.625000;0.630410;,
  0.375000;0.548790;,
  0.375000;0.564070;,
  0.390620;0.564070;,
  0.390620;0.548790;,
  0.406250;0.564070;,
  0.406250;0.548790;,
  0.421880;0.564070;,
  0.421880;0.548790;,
  0.437500;0.564070;,
  0.437500;0.548790;,
  0.453120;0.564070;,
  0.453120;0.548790;,
  0.468750;0.564070;,
  0.468750;0.548790;,
  0.484380;0.564070;,
  0.484380;0.548790;,
  0.500000;0.564070;,
  0.500000;0.548790;,
  0.515620;0.564070;,
  0.515620;0.548790;,
  0.531250;0.564070;,
  0.531250;0.548790;,
  0.546880;0.564070;,
  0.546880;0.548790;,
  0.562500;0.564070;,
  0.562500;0.548790;,
  0.578120;0.564070;,
  0.578120;0.548790;,
  0.593750;0.564070;,
  0.593750;0.548790;,
  0.609380;0.564070;,
  0.609380;0.548790;,
  0.625000;0.564070;,
  0.625000;0.548790;,
  0.609380;0.575880;,
  0.625000;0.575880;,
  0.593750;0.575880;,
  0.578120;0.575880;,
  0.562500;0.575880;,
  0.546880;0.575880;,
  0.531250;0.575880;,
  0.515620;0.575880;,
  0.500000;0.575880;,
  0.484380;0.575880;,
  0.468750;0.575880;,
  0.453120;0.575880;,
  0.437500;0.575880;,
  0.421880;0.575880;,
  0.406250;0.575880;,
  0.390620;0.575880;,
  0.375000;0.575880;,
  0.609380;0.586730;,
  0.625000;0.586730;,
  0.593750;0.586730;,
  0.578120;0.586730;,
  0.562500;0.586730;,
  0.546880;0.586730;,
  0.531250;0.586730;,
  0.515620;0.586730;,
  0.500000;0.586730;,
  0.484380;0.586730;,
  0.468750;0.586730;,
  0.453120;0.586730;,
  0.437500;0.586730;,
  0.421880;0.586730;,
  0.406250;0.586730;,
  0.390620;0.586730;,
  0.375000;0.586730;,
  0.609380;0.630410;,
  0.625000;0.630410;,
  0.593750;0.630410;,
  0.578120;0.630410;,
  0.562500;0.630410;,
  0.546880;0.630410;,
  0.531250;0.630410;,
  0.515620;0.630410;,
  0.500000;0.630410;,
  0.484380;0.630410;,
  0.468750;0.630410;,
  0.453120;0.630410;,
  0.437500;0.630410;,
  0.421880;0.630410;,
  0.406250;0.630410;,
  0.390620;0.630410;,
  0.375000;0.630410;,
  0.390620;0.564070;,
  0.375000;0.564070;,
  0.406250;0.564070;,
  0.421880;0.564070;,
  0.437500;0.564070;,
  0.453120;0.564070;,
  0.468750;0.564070;,
  0.484380;0.564070;,
  0.500000;0.564070;,
  0.515620;0.564070;,
  0.531250;0.564070;,
  0.546880;0.564070;,
  0.562500;0.564070;,
  0.578120;0.564070;,
  0.593750;0.564070;,
  0.609380;0.564070;,
  0.625000;0.564070;,
  0.644360;0.903540;,
  0.610490;0.954240;,
  0.500000;0.837500;,
  0.559790;0.988110;,
  0.500000;1.000000;,
  0.440210;0.988110;,
  0.389520;0.954240;,
  0.355640;0.903540;,
  0.343750;0.843750;,
  0.355640;0.783960;,
  0.389520;0.733260;,
  0.440210;0.699390;,
  0.500000;0.687500;,
  0.559790;0.699390;,
  0.610490;0.733260;,
  0.644360;0.783960;,
  0.656250;0.843750;,
  0.625000;0.575880;,
  0.609380;0.575880;,
  0.593750;0.575880;,
  0.578120;0.575880;,
  0.562500;0.575880;,
  0.546880;0.575880;,
  0.531250;0.575880;,
  0.515620;0.575880;,
  0.500000;0.575880;,
  0.484380;0.575880;,
  0.468750;0.575880;,
  0.453120;0.575880;,
  0.437500;0.575880;,
  0.421880;0.575880;,
  0.406250;0.575880;,
  0.390620;0.575880;,
  0.375000;0.575880;,
  0.625000;0.586730;,
  0.609380;0.586730;,
  0.593750;0.586730;,
  0.578120;0.586730;,
  0.562500;0.586730;,
  0.546880;0.586730;,
  0.531250;0.586730;,
  0.515620;0.586730;,
  0.500000;0.586730;,
  0.484380;0.586730;,
  0.468750;0.586730;,
  0.453120;0.586730;,
  0.437500;0.586730;,
  0.421880;0.586730;,
  0.406250;0.586730;,
  0.390620;0.586730;,
  0.375000;0.586730;,
  0.609380;0.633440;,
  0.609380;0.635080;,
  0.625000;0.635080;,
  0.625000;0.633440;,
  0.593750;0.633440;,
  0.593750;0.635080;,
  0.578120;0.633440;,
  0.578120;0.635080;,
  0.562500;0.633440;,
  0.562500;0.635080;,
  0.546880;0.633440;,
  0.546880;0.635080;,
  0.531250;0.633440;,
  0.531250;0.635080;,
  0.515620;0.633440;,
  0.515620;0.635080;,
  0.500000;0.633440;,
  0.500000;0.635080;,
  0.484380;0.633440;,
  0.484380;0.635080;,
  0.468750;0.633440;,
  0.468750;0.635080;,
  0.453120;0.633440;,
  0.453120;0.635080;,
  0.437500;0.633440;,
  0.437500;0.635080;,
  0.421880;0.633440;,
  0.421880;0.635080;,
  0.406250;0.633440;,
  0.406250;0.635080;,
  0.390620;0.633440;,
  0.390620;0.635080;,
  0.375000;0.633440;,
  0.375000;0.635080;,
  0.625000;0.676210;,
  0.609380;0.675870;,
  0.593750;0.675390;,
  0.578120;0.674850;,
  0.562500;0.674320;,
  0.546880;0.673880;,
  0.531250;0.673610;,
  0.515620;0.673540;,
  0.500000;0.673680;,
  0.484380;0.674020;,
  0.468750;0.674500;,
  0.453120;0.675050;,
  0.437500;0.675580;,
  0.421880;0.676020;,
  0.406250;0.676290;,
  0.390620;0.676360;,
  0.375000;0.676210;,
  0.625000;0.681370;,
  0.625000;0.688440;,
  0.609380;0.688440;,
  0.609380;0.681210;,
  0.593750;0.688440;,
  0.593750;0.680950;,
  0.578120;0.688440;,
  0.578120;0.680630;,
  0.562500;0.688440;,
  0.562500;0.680310;,
  0.546880;0.688440;,
  0.546880;0.680020;,
  0.531250;0.688440;,
  0.531250;0.679820;,
  0.515620;0.688440;,
  0.515620;0.679740;,
  0.500000;0.688440;,
  0.500000;0.679780;,
  0.484380;0.688440;,
  0.484380;0.679940;,
  0.468750;0.688440;,
  0.468750;0.680200;,
  0.453120;0.688440;,
  0.453120;0.680510;,
  0.437500;0.688440;,
  0.437500;0.680840;,
  0.421880;0.688440;,
  0.421880;0.681120;,
  0.406250;0.688440;,
  0.406250;0.681320;,
  0.390620;0.688440;,
  0.390620;0.681410;,
  0.375000;0.688440;,
  0.375000;0.681370;;
 }
}