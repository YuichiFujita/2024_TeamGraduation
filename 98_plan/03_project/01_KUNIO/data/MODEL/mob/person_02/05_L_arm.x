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
 55;
 11.26038;3.95361;6.20943;,
 23.46693;6.81387;0.80070;,
 7.80660;7.21530;1.09674;,
 50.96607;-3.33249;4.61406;,
 39.59280;-0.42474;5.26293;,
 38.97891;-3.83637;5.10597;,
 51.16329;2.08887;4.65000;,
 39.59520;3.03942;5.09787;,
 22.75575;-6.64224;0.55233;,
 23.44701;-4.54986;6.67212;,
 11.65548;-5.49354;5.98758;,
 39.99153;2.59248;-4.69725;,
 39.96450;4.36785;0.44424;,
 51.35163;3.74697;0.36900;,
 51.78951;1.73805;-4.34280;,
 39.54714;-1.67952;-5.40090;,
 51.80478;-1.96788;-4.96527;,
 23.93433;3.87351;6.65730;,
 13.01352;0.02907;7.58442;,
 24.40563;-0.33252;7.07100;,
 23.64678;4.30671;-5.41275;,
 23.26341;0.36132;-7.05267;,
 9.79815;0.96441;-7.58442;,
 8.52774;5.18940;-5.92470;,
 38.72772;-5.61417;0.47748;,
 50.82552;-5.51400;0.35850;,
 50.82552;-5.51400;0.35850;,
 38.72772;-5.61417;0.47748;,
 39.54714;-1.67952;-5.40090;,
 51.80478;-1.96788;-4.96527;,
 23.20698;-3.50520;-5.65791;,
 22.75575;-6.64224;0.55233;,
 11.01552;-3.70326;-6.39789;,
 -0.85116;-2.07327;-3.32079;,
 -1.21017;-0.53283;-3.96492;,
 -0.42804;-3.25683;4.86633;,
 11.65548;-5.49354;5.98758;,
 0.62052;-0.35325;6.00426;,
 -2.23221;2.80113;1.83135;,
 -0.09525;0.92805;5.19978;,
 -1.85217;1.56615;-2.95947;,
 -0.09525;0.92805;5.19978;,
 11.26038;3.95361;6.20943;,
 -0.09525;0.92805;5.19978;,
 0.62052;-0.35325;6.00426;,
 13.01352;0.02907;7.58442;,
 11.26038;3.95361;6.20943;,
 56.34126;0.38514;-1.49184;,
 56.32497;-1.48635;-1.54503;,
 55.85811;-3.33438;0.88257;,
 55.94550;-2.34735;2.78136;,
 56.04309;0.57750;2.83185;,
 56.13639;1.63950;0.74565;,
 56.04309;0.57750;2.83185;,
 51.16329;2.08887;4.65000;;
 
 58;
 3;0,1,2;,
 3;3,4,5;,
 3;4,3,6;,
 3;6,7,4;,
 3;8,9,10;,
 3;11,12,13;,
 3;11,13,14;,
 3;15,11,14;,
 3;15,14,16;,
 3;17,18,19;,
 3;6,13,12;,
 3;6,12,7;,
 3;20,21,22;,
 3;20,22,23;,
 3;5,24,25;,
 3;5,25,3;,
 3;26,27,28;,
 3;26,28,29;,
 3;7,12,1;,
 3;7,1,17;,
 3;12,20,1;,
 3;19,4,7;,
 3;19,7,17;,
 3;24,5,9;,
 3;24,9,8;,
 3;30,27,31;,
 3;20,15,21;,
 4;32,30,31,10;,
 4;33,34,22,32;,
 4;35,33,32,36;,
 4;37,35,36,18;,
 4;37,34,33,35;,
 4;38,39,0,2;,
 4;34,38,2,22;,
 3;39,38,34;,
 4;40,38,2,23;,
 4;34,40,23,22;,
 4;41,34,22,42;,
 4;38,41,42,2;,
 4;38,40,34,41;,
 4;43,44,45,46;,
 3;44,43,34;,
 4;2,1,20,23;,
 4;30,32,22,21;,
 4;12,11,15,20;,
 4;21,28,27,30;,
 4;36,9,19,18;,
 4;9,5,4,19;,
 4;1,0,18,17;,
 4;47,48,29,14;,
 4;48,49,25,29;,
 3;48,47,49;,
 4;49,50,3,25;,
 4;50,51,6,3;,
 4;50,49,47,51;,
 4;52,47,14,13;,
 4;53,52,13,54;,
 3;47,52,53;;
 
 MeshMaterialList {
  9;
  58;
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
   0.800000;0.674400;0.574400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.765490;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.019200;0.310400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.248000;0.119200;0.065600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.420000;0.420000;0.420000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.282400;0.282400;0.282400;1.000000;;
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
  Material {
   0.592800;0.380000;0.122400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  65;
  -0.142744;0.691260;0.708366;,
  -0.183479;0.704492;-0.685584;,
  -0.060526;-0.997421;-0.038582;,
  -0.127223;-0.743540;-0.656478;,
  -0.195353;0.944773;0.263139;,
  0.149742;0.005263;0.988711;,
  0.149914;-0.028828;0.988279;,
  0.323059;0.127678;-0.937727;,
  0.153591;-0.985775;-0.068238;,
  0.210560;0.975029;-0.070588;,
  0.269518;0.500385;-0.822785;,
  0.074189;-0.011948;0.997173;,
  -0.056846;0.139259;0.988623;,
  -0.147763;0.214902;-0.965393;,
  0.077727;-0.004085;0.996966;,
  0.077306;-0.072907;0.994338;,
  0.066094;-0.941933;0.329233;,
  0.031948;-0.742663;-0.668903;,
  0.055639;-0.987869;-0.144981;,
  0.029859;-0.999128;-0.029178;,
  0.077704;0.966973;0.242743;,
  0.105198;0.989551;-0.098602;,
  0.092686;0.649213;-0.754938;,
  0.078223;0.859014;-0.505942;,
  0.088469;0.067937;0.993759;,
  0.060772;0.483496;0.873234;,
  0.088939;0.157950;-0.983434;,
  0.088746;-0.044718;-0.995050;,
  -0.240101;0.630777;0.737883;,
  -0.411444;0.641849;-0.647104;,
  -0.168513;-0.976755;-0.132487;,
  -0.243281;-0.769405;-0.590619;,
  -0.344596;0.869530;0.353795;,
  -0.155080;0.040849;0.987057;,
  -0.318437;-0.041942;-0.947016;,
  0.347052;-0.023291;0.937557;,
  0.383171;-0.497092;0.778511;,
  0.556989;-0.396042;-0.730010;,
  0.408463;-0.912246;-0.031080;,
  0.424356;0.905398;0.013268;,
  0.907896;0.265236;-0.324614;,
  0.041098;0.158221;-0.986548;,
  0.170717;0.921170;0.349716;,
  0.109562;0.935611;0.335601;,
  0.048663;-0.919853;0.389232;,
  0.191325;-0.875809;0.443117;,
  0.000767;-0.831698;-0.555228;,
  0.206802;-0.823465;-0.528336;,
  -0.130211;-0.282615;0.950355;,
  -0.063512;-0.235849;0.969712;,
  -0.973330;-0.162725;0.161710;,
  -0.916221;-0.364225;0.166974;,
  -0.986793;0.079661;0.141042;,
  -0.338507;0.758084;-0.557424;,
  -0.851614;-0.496457;0.168180;,
  -0.822272;-0.544629;0.165073;,
  0.207539;-0.965405;0.157864;,
  -0.844729;-0.516142;0.141529;,
  0.057614;-0.140094;0.988460;,
  0.983789;-0.013646;0.178807;,
  0.992751;-0.045301;0.111322;,
  0.996117;-0.076700;0.043209;,
  0.998343;-0.016557;0.055108;,
  0.363515;0.844194;0.393946;,
  0.996811;0.043648;0.066800;;
  58;
  3;0,20,4;,
  3;6,11,15;,
  3;11,6,5;,
  3;5,24,11;,
  3;19,16,2;,
  3;23,21,9;,
  3;23,9,10;,
  3;26,41,10;,
  3;26,10,7;,
  3;25,12,14;,
  3;42,9,21;,
  3;42,21,43;,
  3;22,27,13;,
  3;22,13,1;,
  3;44,18,8;,
  3;44,8,45;,
  3;8,18,46;,
  3;8,46,47;,
  3;43,21,20;,
  3;43,20,25;,
  3;21,22,20;,
  3;14,11,24;,
  3;14,24,25;,
  3;18,44,16;,
  3;18,16,19;,
  3;17,18,19;,
  3;22,26,27;,
  4;3,17,19,2;,
  4;31,34,13,3;,
  4;30,31,3,2;,
  4;33,48,49,12;,
  4;50,51,52,52;,
  4;32,28,0,4;,
  4;53,53,53,13;,
  3;54,55,51;,
  4;29,32,4,1;,
  4;34,29,1,13;,
  4;56,56,56,56;,
  4;32,28,0,4;,
  4;55,57,51,54;,
  4;28,33,12,0;,
  3;50,54,51;,
  4;4,20,22,1;,
  4;17,3,13,27;,
  4;21,23,26,22;,
  4;27,26,18,17;,
  4;49,58,14,12;,
  4;58,15,11,14;,
  4;20,0,12,25;,
  4;40,37,7,10;,
  4;37,38,8,47;,
  3;59,59,60;,
  4;38,36,45,8;,
  4;36,35,5,6;,
  4;61,60,40,62;,
  4;39,40,10,9;,
  4;63,39,9,42;,
  3;40,64,62;;
 }
 MeshTextureCoords {
  55;
  0.099710;0.637760;,
  0.126380;0.677840;,
  0.124140;0.627150;,
  0.072840;0.802410;,
  0.084310;0.760420;,
  0.066350;0.760580;,
  0.103740;0.799890;,
  0.102370;0.757840;,
  0.020410;0.690390;,
  0.052590;0.685200;,
  0.033740;0.651850;,
  0.156590;0.758660;,
  0.128020;0.758270;,
  0.129550;0.798190;,
  0.156680;0.798560;,
  0.173080;0.759130;,
  0.171180;0.799140;,
  0.094400;0.681040;,
  0.070830;0.648090;,
  0.073870;0.685260;,
  0.159440;0.679480;,
  0.180930;0.680570;,
  0.183730;0.636380;,
  0.161300;0.629850;,
  0.041040;0.765190;,
  0.047700;0.804280;,
  0.210270;0.802320;,
  0.217850;0.763950;,
  0.188820;0.760740;,
  0.183960;0.800760;,
  0.201480;0.683590;,
  0.236170;0.689460;,
  0.207220;0.643850;,
  0.207220;0.643850;,
  0.183730;0.636380;,
  0.048570;0.647600;,
  0.048570;0.647600;,
  0.070830;0.648090;,
  0.124140;0.627150;,
  0.099710;0.637760;,
  0.161300;0.629850;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.049860;0.318880;,
  0.035420;0.324050;,
  0.035420;0.324050;,
  0.049860;0.318880;,
  0.156680;0.798560;,
  0.183960;0.800760;,
  0.047700;0.804280;,
  0.072840;0.802410;,
  0.103740;0.799890;,
  0.129550;0.798190;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}