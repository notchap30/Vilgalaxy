def map(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

print(map(197.0,197.0,197.0,10.0,20.0))