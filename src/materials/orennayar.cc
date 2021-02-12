#include "materials/orennayar.h"

OrenNayar::OrenNayar(double roughness, Colour col) 
  : BSDF(col) {
  // roughness = Radians(sigma);
  double sigma2 = roughness * roughness;
  A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
  B = 0.45f * sigma2 / (sigma2 + 0.09f);
}


Colour OrenNayar::eval(BSDFRec &bRec) {
  Vec wi = toLocalFrame(bRec.wi, bRec.n);
  Vec wo = toLocalFrame(bRec.wo, bRec.n);
  // std::cout << "---------------- START-----------------" << std::endl;
  // std::cout << bRec.wi << "---> " <<wi << std::endl;
  // std::cout << bRec.wo << "---> " <<wo << std::endl;
  // std::cout << "----------------  END -----------------" << std::endl;


  double sinThetaI = sinTheta(wi);
  double sinThetaO = sinTheta(wo);
  // Compute cosine term of Oren-Nayar model
  double maxCos = 0;
  if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
      double sinPhiI = sinPhi(wi), cosPhiI = cosPhi(wi);
      double sinPhiO = sinPhi(wo), cosPhiO = cosPhi(wo);
      double dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
      maxCos = max(0, dCos);
  }

  // Compute sine and tangent terms of Oren-Nayar model
  double sinAlpha, tanBeta;
  if (absCosTheta(wi) > absCosTheta(wo)) {
      sinAlpha = sinThetaO;
      tanBeta = sinThetaI / absCosTheta(wi);
  } else {
      sinAlpha = sinThetaI;
      tanBeta = sinThetaO / absCosTheta(wo);
  }
  // printf("maxCos: %f, sinAlpha: %f, tanBeta: %f\n", maxCos , sinAlpha , tanBeta);
  return col * INV_PI * (A + B * maxCos * sinAlpha * tanBeta);
}

Colour OrenNayar::sample(BSDFRec &bRec) {
  bRec.wi = bRec.rng.randomVectorCosineHemisphere(bRec.n);
  double cosTheta = dot(bRec.n, bRec.wi);
  return (cosTheta * eval(bRec)) / pdf(bRec);
}

double OrenNayar::pdf(BSDFRec &bRec) {
  return dot(bRec.wi, bRec.n) * INV_PI;
}
