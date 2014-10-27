extern "C" {

float evaluate_0(const float* f) {
  if (f[770] < 0.892776) {
    if (f[831] < 0.53155) {
      return 0.915533;
    } else {
      return 0.221931;
    }
  } else {
    if (f[135] < 0.876372) {
      return 0.258631;
    } else {
      return 0.888477;
    }
  }
}

float evaluate_1(const float* f) {
  if (f[739] < 0.477916) {
    if (f[697] < 0.794339) {
      return 0.51592;
    } else {
      return 0.458784;
    }
  } else {
    if (f[29] < 0.420066) {
      return 0.945054;
    } else {
      return 0.894636;
    }
  }
}

float evaluate(const float* f) {
  float result = 0.0;
  result += evaluate_0(f);
  result += evaluate_1(f);
  return result;
}

}
