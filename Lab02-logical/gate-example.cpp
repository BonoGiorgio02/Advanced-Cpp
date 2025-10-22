int main(int argc, char* argv[]) {
  auto sim = gate::agenda();
  
  auto a   = gate::wire();
  auto b   = gate::wire();
  auto c   = gate::wire();
  auto d   = gate::wire();
  
  auto A   = gate::AND(sim, a, b, c);
  auto B   = gate::AND(sim, b, c, d);

  return 0;
}
				  
