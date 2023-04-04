#include "Join.hpp"
#include "Page.hpp"
#include "Mem.hpp"
#include <vector>
#include <iostream>

using namespace std;

/*
 * Input: Disk, Memory, Disk page ids for left relation, Disk page ids for right relation
 * Output: Vector of Buckets of size (MEM_SIZE_IN_PAGE - 1) after partition
 */
vector<Bucket> partition(Disk* disk, Mem* mem, pair<uint, uint> left_rel,
                         pair<uint, uint> right_rel) {
	// TODO: implement partition phase

	// output = a vector of buckets of size MEM_SIZE_IN PAGE - 1
	// each bucket stores all the disk page ids and number of records for the 
	// left and right relations for one partition

	// B-1 buckets
	// reserve one page for input from disk 

	/*
	1. Load each relation page by page
	2. Also need to iterate over records in each page
	3. Hash each record to the correct bucket and all to the corresponding page
	4. If a page is full, write it to disk
	5. If a relation is exhausted (done) also write partially full pages to disk
	6. Clear the pages after writing to disk
	*/
	// 0 -> MEM_SIZE_IN_PAGE - 2

	// auto temp = mem.loadFromDisk(disk, left_rel.first, left_rel.second);

	// pair<uint, uint> right_rel - 0 -10
	// pair<uint, uint> left_rel  - 11 -20
	
	unsigned int left_counter = left_rel.first;
	unsigned int left_max = left_rel.second;
	unsigned int right_counter = right_rel.first;
	unsigned int right_max = right_rel.second;

	for (int i = 0; i < MEM_SIZE_IN_PAGE - 1 && left_counter <= left_max; i++){
		mem->loadFromDisk(disk, left_counter, i);
		++left_counter;
	}

	cout << "finished loading from disk" << endl;

	vector<vector<Record>> h1_hash_results((MEM_SIZE_IN_PAGE - 1), vector<Record>());
	
	// iterate through each record in page
	for (int i = 0; i < MEM_SIZE_IN_PAGE - 1; ++i) {
		Page* mem_page = mem->mem_page(i);
		for (int j = 0; j < mem_page->size(); ++j) {
			Record record = mem_page->get_record(j);
			
			uint hash_value = record.partition_hash() % (MEM_SIZE_IN_PAGE - 1);
			h1_hash_results[hash_value].push_back(record);

		}
	}
	
	cout << "iterated through each record in page" << endl;
	
	vector<Bucket> partitions((MEM_SIZE_IN_PAGE - 1), Bucket(disk));
	
	Page* output_buffer = mem->mem_page(MEM_SIZE_IN_PAGE - 1);

	cout << "BEFORE ADDING TO BUCKETS" << endl;

	// for (auto x: h1_hash_results){
	// 	cout << x << endl;
	// }

	for (int i = 0; i < MEM_SIZE_IN_PAGE - 1; ++i) {
		vector<Record> sameHashRecords = h1_hash_results[i];
		
		// cout << "here" << endl;

		for (int j = 0; j < sameHashRecords.size(); ++j) {
			output_buffer->loadRecord(sameHashRecords[j]);
			// cout << "output buffer " << endl;
		}

		// cout << "here2" << endl;
		if (!output_buffer->empty()) {
			unsigned int disk_page_id = mem->flushToDisk(disk, MEM_SIZE_IN_PAGE - 1);
			partitions[i].add_left_rel_page(disk_page_id);
			
		}

		output_buffer->reset();
	}

	cout << "FINISHED ADDING TO BUCKETS" << endl;
	 
	return partitions;
}

/*
 * Input: Disk, Memory, Vector of Buckets after partition
 * Output: Vector of disk page ids for join result
 */
vector<uint> probe(Disk* disk, Mem* mem, vector<Bucket>& partitions) {
	// TODO: implement probe phase
	vector<uint> disk_pages; // placeholder
	return disk_pages;
}
