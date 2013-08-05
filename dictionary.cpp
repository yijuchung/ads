#include "dictionary.h"

using namespace	std;

int main(int argc ,char *argv[] )
{
	bool bInputMode = false;

	int n = 0;
	int s = 3;
	int iRandOrder = 10;

	if( argc < 2 ){
		cout << "Usage : -r/u <file-name>" << endl;
		return 0;
	}else{
		//cout << argv[2];
		if( strcmp( argv[1],"-r") == 0 ){
			// random mode
			s = atoi(argv[2]);
			iRandOrder = atoi(argv[3]);
		}else if( strcmp( argv[1],"-u") == 0 && argc == 3){
			// user input mode
			bInputMode = true;
		}else{
			cout << "Usage : -r/u <file-name>" << endl;
			return 0;
		}
	}
	
	vector<pair<int,int>> vCmd;
	vector<pair<int,int>>::iterator itCmd;

	if(!bInputMode){
		cout << "input a number n :" << endl;
		cin >> n;

		for(int i = 1;i <= n;i++){
			vCmd.push_back(pair<int,int>(i,2*i));
		}

		clock_t cStart, cDiff;
		clock_t cRoundS, cRoundE;
		vector<int> vInsertTime(6);
		vector<int> vSearchTime(6);

		//---------------------random mode---------------------------------
		
		/*
		for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ )
			cout << " " << itCmd->first << endl;
		*/
		srand(unsigned(time(NULL)));
		//int iRandOrder = rand()%(n-2) + 2;

		for( int iCount = 0 ;iCount < 10; iCount++){
			//random_shuffle( vCmd.begin(), vCmd.end());
			
			//iRandOrder = rand()%8 + 3;
			//s = rand()%8 + 3;
			//if( s%2 == 0 )
			//	s++;

			//for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ )
			//	cout << " " << itCmd->first << endl;

			//-----------------------------AVL---------------------------------
			//cout << "start " << iCount << " round AVL" << endl;
			itCmd = vCmd.begin();
			
			//cRoundS = clock();
			cStart = clock();
			
			//avNode *avlTree = new avNode(itCmd->first);
			avTree *avt = new avTree();
			//itCmd++;

			// insert time
			while( itCmd != vCmd.end() ){
				avt->insert( itCmd->first, itCmd->second );
				itCmd++;
			}

			//avt->printInorder( avt->avRoot );

			cDiff = clock()-cStart;
			vInsertTime[0] += cDiff*1000 / CLOCKS_PER_SEC;

			itCmd = vCmd.begin();
			
			cStart = clock();
			// search time
			while( itCmd != vCmd.end() ){
				avt->find( itCmd->first );
				itCmd++;
			}

			cDiff = clock()-cStart;
			vSearchTime[0] += cDiff*1000 / CLOCKS_PER_SEC;

			avt->release( avt->avRoot );

			//cRoundE = clock()-cRoundS;
			//cout << "This round took " << (cRoundE*1000 / CLOCKS_PER_SEC) << "(ms)" << endl;
			//delete avlTree;
			//---------------------------------------------------------------------

			//-----------------------------AVLHash---------------------------------
			
			//cout << "start " << iCount << " round AVLHash" << endl;
			vector<avTree *> vAVLtree(s);
			
			//cRoundS = clock();
			// insert time
			cStart = clock();

			for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ ){
				if( vAVLtree[itCmd->first%s] == NULL || vAVLtree[itCmd->first%s]->avRoot == NULL )
					vAVLtree[itCmd->first%s] = new avTree(itCmd->first,itCmd->second);
				else{
					vAVLtree[itCmd->first%s]->insert( itCmd->first,itCmd->second );
				}
			}

			cDiff = clock()-cStart;
			vInsertTime[1] += cDiff*1000 / CLOCKS_PER_SEC;

			cStart = clock();
			itCmd = vCmd.begin();
			// search time
			while( itCmd != vCmd.end() ){
				vAVLtree[itCmd->first%s]->find( itCmd->second );
				itCmd++;
			}
			
			cDiff = clock()-cStart;
			vSearchTime[1] += cDiff*1000 / CLOCKS_PER_SEC;

			for( int i = 0; i < vAVLtree.size() ; i++ ){
				vAVLtree[i]->release(vAVLtree[i]->avRoot);
				//delete vAVLtree[i];
			}

			//cRoundE = clock()-cRoundS;
			//cout << "This round took " << (cRoundE*1000 / CLOCKS_PER_SEC) << "(ms)" << endl;
			
			//---------------------------------------------------------------------

			//-----------------------------RedBlack----------------------------
			//cout << "start " << iCount << " round RedBlack" << endl;
			map<int,int> mRBTree;

			//cRoundS = clock();
			itCmd = vCmd.begin();
			// insert time

			cStart = clock();

			for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ ){
				mRBTree.insert( pair<int,int>(itCmd->first,itCmd->second) );
			}
			
			cDiff = clock()-cStart;
			vInsertTime[5] += cDiff*1000 / CLOCKS_PER_SEC;

			itCmd = vCmd.begin();

			cStart = clock();
			// search time
			while( itCmd != vCmd.end() ){
				mRBTree.find(itCmd->first);
				itCmd++;
			}

			cDiff = clock()-cStart;
			vSearchTime[5] += cDiff*1000 / CLOCKS_PER_SEC;

			mRBTree.clear();

			//cRoundE = clock()-cRoundS;
			//cout << "This round took " << (cRoundE*1000 / CLOCKS_PER_SEC) << "(ms)" << endl;
			//---------------------------------------------------------------------

			//-----------------------------RedBlackHash----------------------------
			//cout << "start " << iCount << " round RedBlackHash" << endl;
			vector< map<int,int> > vmRBTree(s);

			//cRoundS = clock();
			itCmd = vCmd.begin();
			// insert time

			cStart = clock();

			for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ ){
				vmRBTree[itCmd->first%s].insert( pair<int,int>(itCmd->first,itCmd->second) );
			}
			
			cDiff = clock()-cStart;
			vInsertTime[2] += cDiff*1000 / CLOCKS_PER_SEC;

			itCmd = vCmd.begin();

			cStart = clock();
			// search time
			while( itCmd != vCmd.end() ){
				vmRBTree[itCmd->first%s].find(itCmd->first);
				itCmd++;
			}

			cDiff = clock()-cStart;
			vSearchTime[2] += cDiff*1000 / CLOCKS_PER_SEC;

			for( int i = 0; i < vmRBTree.size() ; i++ ){
				vmRBTree[i].clear();
			}

			//cRoundE = clock()-cRoundS;
			//cout << "This round took " << (cRoundE*1000 / CLOCKS_PER_SEC) << "(ms)" << endl;
			//---------------------------------------------------------------------
			
			//----------------------------BTree------------------------------------
			//cout << "start " << iCount << " round BTree" << endl;
			
			//cRoundS = clock();
			//iRandOrder = rand()%10 + 3;

			BTree *pBT = new BTree(iRandOrder);
			itCmd = vCmd.begin();

			cStart = clock();
			//cout << "----------------------new tree--------------------------" << endl;
			while( itCmd != vCmd.end() ){
				pBT->insert( itCmd->first,itCmd->second );
				//pBT->printBTree();
				itCmd++;
			}

			cDiff = clock()-cStart;
			//cout << cDiff << endl;
			vInsertTime[3] += cDiff*1000 / CLOCKS_PER_SEC;

			itCmd = vCmd.begin();
			
			cStart = clock();

			while( itCmd != vCmd.end() ){
				pBT->search( itCmd->first );
				itCmd++;
			}

			cDiff = clock()-cStart;
			vSearchTime[3] += cDiff*1000 / CLOCKS_PER_SEC;

			pBT->release();
			//delete pBT;
			//cRoundE = clock()-cRoundS;
			//cout << "This round took " << (cRoundE*1000 / CLOCKS_PER_SEC) << "(ms)" << endl;
			//---------------------------------------------------------------------
						
			//----------------------------BTreeHash------------------------------------
			//cout << "start " << iCount << " round BTreeHash" << endl;
			//int iRandOrder = rand()%(n-2) + 2;

			vector<BTree *> vBTree(s);

			//cRoundS = clock();
			cStart = clock();
			//BTree *pBT = new BTree(iRandOrder);
			//itCmd = vCmd.begin();

			for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ ){
				if( vBTree[itCmd->first%s] == NULL || vBTree[itCmd->first%s]->bRoot == NULL ){
					vBTree[itCmd->first%s] = new BTree(iRandOrder);
					vBTree[itCmd->first%s]->insert( itCmd->first,itCmd->second );
				}
				else
					vBTree[itCmd->first%s]->insert( itCmd->first,itCmd->second );
			}

			cDiff = clock()-cStart;
			vInsertTime[4] += cDiff*1000 / CLOCKS_PER_SEC;

			itCmd = vCmd.begin();

			cStart = clock();

			while( itCmd != vCmd.end() ){
				vBTree[itCmd->first%s]->search(itCmd->second);
				itCmd++;
			}

			cDiff = clock()-cStart;
			vSearchTime[4] += cDiff*1000 / CLOCKS_PER_SEC;

			for( int i = 0; i < vBTree.size() ; i++ ){
				vBTree[i]->release();
				//delete vBTree[i];
			}

			//cRoundE = clock()-cRoundS;
			//cout << "This round took " << (cRoundE*1000 / CLOCKS_PER_SEC) << "(ms)" << endl;
			//---------------------------------------------------------------------
		}

		cout << "AVL avg insert time : " << (vInsertTime[0]/10) << endl;
		cout << "AVL avg search time : " << (vSearchTime[0]/10) << endl;

		cout << "AVLHash avg insert time : " << (vInsertTime[1]/10) << endl;
		cout << "AVLHash avg search time : " << (vSearchTime[1]/10) << endl;

		cout << "RBTree avg insert time : " << vInsertTime[5]/10 << endl;
		cout << "RBTree avg search time : " << vSearchTime[5]/10 << endl;

		cout << "RBHash avg insert time : " << vInsertTime[2]/10 << endl;
		cout << "RBHash avg search time : " << vSearchTime[2]/10 << endl;

		cout << "BTree avg insert time : " << vInsertTime[3]/10 << endl;
		cout << "BTree avg search time : " << vSearchTime[3]/10 << endl;

		cout << "BTreeHash avg insert time : " << vInsertTime[4]/10 << endl;
		cout << "BTreeHash avg search time : " << vSearchTime[4]/10 << endl;
	}else{
		// input mode
		//cout << "input mode" << endl;
		//FILE *fp = fopen( argv[2], "r");
		ifstream infile;
		infile.open(argv[2]);
		
		string sInput;
		getline(infile,sInput);

		n = atoi(sInput.c_str());

		for( int i = 0;i<n;i++ ){
			getline(infile,sInput);

			istringstream iss(sInput);
			string sKey;
			iss >> sKey;
			string sVal;
			iss >> sVal;

			vCmd.push_back(pair<int,int>(atoi(sKey.c_str()),atoi(sVal.c_str())));
		}

		infile.close();
		
		//---------------------AVL------------------------
		//cout << "start AVL" << endl;
		itCmd = vCmd.begin();

		avTree *avt = new avTree();
		
		while( itCmd != vCmd.end() ){
			avt->insert( itCmd->first, itCmd->second );
			itCmd++;
		}

		ofstream redirect_file("AVL_inorder.out");
		streambuf * strm_buffer = cout.rdbuf();
		
		cout.rdbuf(redirect_file.rdbuf());
		avt->printInorder( avt->avRoot );
		cout.rdbuf(strm_buffer);
		
		redirect_file.close();

		redirect_file.open("AVL_postorder.out");
		strm_buffer = cout.rdbuf();
		cout.rdbuf(redirect_file.rdbuf());
		
		avt->printPostorder( avt->avRoot );
		
		cout.rdbuf(strm_buffer);

		redirect_file.close();

		avt->release( avt->avRoot );
		//------------------------------------------------

		//--------------------AVLHash---------------------
		cout << "start AVL Hash" << endl;
		vector<avTree *> vAVLtree(s);

		for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ ){
			if( vAVLtree[itCmd->first%s] == NULL || vAVLtree[itCmd->first%s]->avRoot == NULL )
				vAVLtree[itCmd->first%s] = new avTree(itCmd->first, itCmd->second);
			else{
				vAVLtree[itCmd->first%s]->insert( itCmd->first, itCmd->second );
			}
		}

		redirect_file.open("AVLHash_inorder.out");
		strm_buffer = cout.rdbuf();
		cout.rdbuf(redirect_file.rdbuf());

		for( int i = 0; i < vAVLtree.size() ; i++ ){
			
			vAVLtree[i]->printInorder(vAVLtree[i]->avRoot);
			cout << endl;
		}

		cout.rdbuf(strm_buffer);
		redirect_file.close();

		redirect_file.open("AVLHash_postorder.out");
		strm_buffer = cout.rdbuf();
		cout.rdbuf(redirect_file.rdbuf());

		for( int i = 0; i < vAVLtree.size() ; i++ ){
			
			vAVLtree[i]->printPostorder(vAVLtree[i]->avRoot);
			cout << endl;
		}

		cout.rdbuf(strm_buffer);
		redirect_file.close();
		
		for( int i = 0; i < vAVLtree.size() ; i++ ){
			vAVLtree[i]->release(vAVLtree[i]->avRoot);
		}
		//------------------------------------------------
		
		//--------------------BTree-----------------------
		cout << "start BTree" << endl;
		BTree *pBT = new BTree(3);
		itCmd = vCmd.begin();

		while( itCmd != vCmd.end() ){
			pBT->insert( itCmd->first,itCmd->second );
			itCmd++;
		}
		
		redirect_file.open("BTree_level.out");
		strm_buffer = cout.rdbuf();
		
		cout.rdbuf(redirect_file.rdbuf());
		pBT->printBTree();
		cout.rdbuf(strm_buffer);
		redirect_file.close();

		redirect_file.open("BTree_sorted.out");
		strm_buffer = cout.rdbuf();
		cout.rdbuf(redirect_file.rdbuf());
		pBT->printSortedBTree(pBT->bRoot);
		cout.rdbuf(strm_buffer);
		redirect_file.close();
		pBT->release();
		//------------------------------------------------

		//--------------------BTreeHash-------------------
		cout << "start BTree Hash" << endl;
		vector<BTree *> vBTree(s);

		for( itCmd = vCmd.begin(); itCmd != vCmd.end() ; itCmd++ ){
			if( vBTree[itCmd->first%s] == NULL || vBTree[itCmd->first%s]->bRoot == NULL ){
				vBTree[itCmd->first%s] = new BTree(3);
				vBTree[itCmd->first%s]->insert( itCmd->first,itCmd->second );
			}
			else
				vBTree[itCmd->first%s]->insert( itCmd->first,itCmd->second );
		}

		redirect_file.open("BTreeHash_level.out");
		strm_buffer = cout.rdbuf();
		cout.rdbuf(redirect_file.rdbuf());

		for( int i = 0; i < vBTree.size() ; i++ ){
			vBTree[i]->printBTree();
			
			cout << endl;
			//vBTree[i]->release();
		}

		cout.rdbuf(strm_buffer);
		redirect_file.close();
		redirect_file.open("BTreeHash_sorted.out");
		strm_buffer = cout.rdbuf();
		cout.rdbuf(redirect_file.rdbuf());

		for( int i = 0; i < vBTree.size() ; i++ ){
			
			vBTree[i]->printSortedBTree(vBTree[i]->bRoot);
			cout << endl;
			vBTree[i]->release();
		}

		cout.rdbuf(strm_buffer);
		redirect_file.close();
		//------------------------------------------------
		
		
	}

	return 0;
}