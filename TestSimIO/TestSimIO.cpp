﻿
#include <iostream>
#include "../SimIO/cg"
#include "../SimIO/ann"
#include "../SimIO/opt"
#include "../SimIO/utilities.h"


using namespace std;

int main()
{
// part I: an example of a DAG building
	using namespace SIMIO::CG;
	Status st;
	auto session = GraphCreate<VVAL>();
	auto x  = VFUN::Placeholder(st, session, 3, "x");
	auto c1 = VFUN::Constant(st, session, { 1.1, 2.3, 4.5 }, "c1");
	auto a = VFUN::Add(st, session, x, c1, "a");
	auto y = VFUN::Sub(st, session, VFUN::Square(st, session, a, "a^2"), x, "y = a^2 - x");
	auto y1 = VFUN::Relu(st, session, y, "Relu(y)");
	auto y2 = VFUN::Cube(st, session, y1, "Cube(y1)");
	cout << session->size() << endl;
	VFUN::Feed(st, x, VVAL::Ones(3) * 2.4);
	st = y2->calc();

	cout << st.description << endl;
	if(st.state) {
		cout << "x:" << endl;
		cout << x->result() << endl;
		cout << "c1:" << endl;
		cout << c1->result() << endl;
		cout << "a = x + c1:" << endl;
		cout << a->result() << endl;
		cout << "y = (x + c1) ^ 2 - x:" << endl;
		cout << y->result() << endl;
		cout << "y1 = activation(y):" << endl;
		cout << y1->result() << endl;
		cout << "y2 = f(y1):" << endl;
		cout << y2->result() << endl;
		auto grads = VFUN::Grad(st, session, x, y2);
		cout << st.description << endl;
		cout << "dy / dx = " << endl;
		cout << grads << endl;
	}
/* part I:
7
Fine
x:
2.4
2.4
2.4
c1:
1.1
2.3
4.5
a = x + c1:
3.5
4.7
6.9
y = (x + c1) ^ 2 - x:
 9.85
19.69
45.21
y1 = activation(y):
 9.85
19.69
45.21
y2 = f(y1):
955.672
7633.74
92406.7
Fine
dy / dx = 
1746.41
9769.95
78487.5
*/

// part II: an example of the fully-connected nn building;
	using namespace SIMIO::NN;
	ANN ann(5, 2);
	ann.add_layer(8);
	ann.add_layer(4);
	ann.add_output();
	ann.feed_input(VVAL::Ones(5));
	Status nnst = ann.status();
	cout << nnst.description << endl;
	cout << ann.evaluate() << endl;
	ann.feed_input(VVAL::Ones(5) * 2);
	nnst = ann.status();
	cout << nnst.description << endl;
	cout << ann.evaluate() << endl;

// part III: optimize a nn using pso
	using namespace SIMIO::TOOLS;
	Status optst;
	vector<uint> layers{ 5, 8, 6, 3 };
	ANNTrainer* trainer = new ANNTrainer(
		3,
        1,
        layers);
	uint dp_num = trainer->design_param_num();
	cout << endl << "It's " << dp_num << " design parameters" << endl;
	uint sample_num = 100;
	vector<VVAL> xs(sample_num);
	vector<VVAL> fxs(sample_num);
	//cout << "samples" << endl;
	for(auto i = 0; i < sample_num; i++) {
		xs[i] = VVAL::Random(3) * 10;
		//cout << "x" << i+1 << endl << xs[i] << endl;
		VVAL tmp = xs[i].array().square() * 0.1;
		fxs[i] = RowVectorXd::Ones(3) * tmp;
		//cout << "fx" << i+1 << endl << fxs[i] << endl;
	}

	trainer->set_sample(xs, fxs);
	using namespace SIMIO::OPT;
	//*/
	PSO pso(
		dp_num,
		dp_num * 5,
		30,
		VVAL::Ones(dp_num) * 0.95,
		-VVAL::Ones(dp_num) * 0.95,
		100,
		-100,
		trainer);
	cout << "to run the optimizer" << endl;
	pso.run_opt();
	cout << "current the loss is " << pso.get_opt_obj() << endl;
	cout << "dp is \n" << pso.get_opt_dp() << endl;
	trainer->update_coefficients(pso.get_opt_dp());
	VVAL test_x(3);
	test_x << 1, 5, 9;
	cout << "test x:\n" << test_x << endl;
	cout << "fx is\n" << trainer->feed_test(test_x) << endl;
	//*/

	//*/
	trainer->update_coefficients(VVAL::Zero(dp_num));
	DE de(
		dp_num,
		dp_num * 5,
		30,
		VVAL::Ones(dp_num) * 0.95,
		-VVAL::Ones(dp_num) * 0.95,
		100,
		-100,
		trainer); // from this test, it seems that pso has better performance than de in such case;
	cout << "to run the optimizer" << endl;
	de.run_opt();
	cout << "current the loss is " << de.get_opt_obj() << endl;
	cout << "dp is \n" << de.get_opt_dp() << endl;
	trainer->update_coefficients(de.get_opt_dp());
	cout << "test x:\n" << test_x << endl;
	cout << "fx is\n" << trainer->feed_test(test_x) << endl;
	//*/

	delete trainer;
/* part III:
It's 124 design parameters
to run the optimizer

Opt step 1 current loss is 14.8739

Opt step 2 current loss is 14.8739

Opt step 3 current loss is 14.8739

Opt step 4 current loss is 14.8739

Opt step 5 current loss is 13.3172

Opt step 6 current loss is 13.3172

Opt step 7 current loss is 12.6161

Opt step 8 current loss is 10.6965

Opt step 9 current loss is 10.6965

Opt step 10 current loss is 10.6965

Opt step 11 current loss is 10.6965

Opt step 12 current loss is 8.15622

Opt step 13 current loss is 8.15622

Opt step 14 current loss is 8.15622

Opt step 15 current loss is 8.15622

Opt step 16 current loss is 8.03594

Opt step 17 current loss is 8.02527

Opt step 18 current loss is 7.66244

Opt step 19 current loss is 7.65518

Opt step 20 current loss is 7.65518

Opt step 21 current loss is 7.29641

Opt step 22 current loss is 7.2839

Opt step 23 current loss is 7.11302

Opt step 24 current loss is 7.11302

Opt step 25 current loss is 6.55475

Opt step 26 current loss is 6.55475

Opt step 27 current loss is 6.55475

Opt step 28 current loss is 6.46948

Opt step 29 current loss is 6.46948

Opt step 30 current loss is 6.46948
current the loss is 6.46948
test x:
1
5
9
fx is
11.2304

*/
	return 0;
}
