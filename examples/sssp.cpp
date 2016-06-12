
#include "core/graph.hpp"

int main(int argc, char ** argv) {
/*
	long i;
	if (argc<3) {
		fprintf(stderr, "usage: sssp [path] [start vertex id] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	VertexId start_vid = atoi(argv[2]);
	long memory_bytes = (argc>=4)?atol(argv[3])*1024l*1024l*1024l:8l*1024l*1024l*1024l;
	long minindex;
	float minvalue;
	long *localminindex=(long *)malloc(sizeof(long)*std::thread::hardware_concurrency());
	float *localminvalue=(float *)malloc(sizeof(float)*std::thread::hardware_concurrency());
	int threadindex;

	Graph graph(path);
	graph.set_memory_bytes(memory_bytes);
	Bitmap * dist_found = graph.alloc_bitmap();
	Bitmap * dist_reach = graph.alloc_bitmap();
	Bitmap * update = graph.alloc_bitmap();
	float *dist=(float *)malloc(sizeof(float)*graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * sizeof(VertexId) );

	dist_found->clear();
	dist_found->set_bit(start_vid);
	dist_reach->clear();

	std::fill(dist, dist+graph.vertices, (float)-1.);
	dist[start_vid] = 0;

	VertexId active_vertices = 1;
	minindex=start_vid;
	minvalue=0;

	double start_time = get_time();
	int iteration = 0;

	while (minvalue!=-1) {

		iteration++;

		update->clear();
		update->set_bit(minindex);

		active_vertices = graph.stream_edges<VertexId>([&](Edge & e){
			if ((dist_found->get_bit(e.target))==0){
				if ((dist[e.target]==-1)|(dist[e.source]+e.weight<dist[e.target])) {
					dist[e.target]=dist[e.source]+e.weight;
					if (~dist_reach->get_bit(e.target))
						dist_reach->set_bit(e.target);
				}
			}
			return 0;
		}, update);

		#pragma omp parallel private(threadindex,i)
		{
			threadindex = omp_get_thread_num();
			localminindex[threadindex]=-1;
			localminvalue[threadindex]=-1;
			for (i=threadindex;i<graph.vertices;i+=std::thread::hardware_concurrency()){
				if (dist_reach->get_bit(i)){
					if ((localminvalue[threadindex]==-1)|(dist[i]<localminvalue[threadindex])) {
						localminvalue[threadindex]=dist[i];
						localminindex[threadindex]=i;
					}
				}
			}
		}

		minindex=-1;
		minvalue=-1;
		for(i=0;i<std::thread::hardware_concurrency();i++){
			if (localminindex[i]!=-1){
				if ((minindex==-1)|localminvalue[i]<minvalue){
					minindex=localminindex[i];
					minvalue=localminvalue[i];
				}
			}
		}

		if (minvalue==-1)
			break;

		dist_found->set_bit(minindex);
		dist_reach->unset_bit(minindex);

		fprintf(stderr,"progress: %d/%d\r", iteration,graph.vertices);
		fflush(stderr);

	}
	double end_time = get_time();

	int discovered_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
		return dist_found->get_bit(i)!=0;
	});
	fprintf(stderr,"discovered %d vertices from %d in %.2f seconds.\n", discovered_vertices, start_vid, end_time - start_time);

	for (i=0;i<graph.vertices;i++)
		if (dist_found->get_bit(i)!=0)
			printf("%d %f\n", i,dist[i]);

	free(localminindex);
	free(localminvalue);
	free(dist);
*/

	long i;
	if (argc<3) {
		fprintf(stderr, "usage: sssp [path] [start vertex id] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	VertexId start_vid = atoi(argv[2]);
	long memory_bytes = (argc>=4)?atol(argv[3])*1024l*1024l*1024l:8l*1024l*1024l*1024l;
	long changeindex;

	Graph graph(path);
	graph.set_memory_bytes(memory_bytes);
	Bitmap * dist_reach = graph.alloc_bitmap();
	float *dist=(float *)malloc(sizeof(float)*graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * sizeof(VertexId) );

	dist_reach->clear();
	dist_reach->set_bit(start_vid);

	std::fill(dist, dist+graph.vertices, (float)-1.);
	dist[start_vid] = 0;

	VertexId active_vertices = 1;

	double start_time = get_time();
	int iteration = 0;

	while (1) {

		iteration++;

		changeindex=0;

		active_vertices = graph.stream_edges<VertexId>([&](Edge & e){
			if ((dist[e.target]==-1)|(dist[e.source]+e.weight<dist[e.target])) {
				dist[e.target]=dist[e.source]+e.weight;
				dist_reach->set_bit(e.target);
				changeindex++;
			}
			return 0;
		}, dist_reach);

		if ((changeindex==0)|(iteration==graph.vertices-1))
			break;

		fprintf(stderr,"progress: %d %d\r", iteration,changeindex);
		fflush(stderr);

	}
	double end_time = get_time();

	int discovered_vertices=0;
	#pragma omp parallel for reduction(+:discovered_vertices)
	for (i=0;i<graph.vertices;i++){
		if (dist[i]!=-1) discovered_vertices++;
	}
	fprintf(stderr,"discovered %d vertices from %d in %.2f seconds.\n", discovered_vertices, start_vid, end_time - start_time);

	for (i=0;i<graph.vertices;i++)
		if (dist[i]!=-1)
			printf("%d %f\n", i,dist[i]);

	free(dist);

/*
	long i;
	if (argc<3) {
		fprintf(stderr, "usage: sssp [path] [start vertex id] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	VertexId start_vid = atoi(argv[2]);
	long memory_bytes = (argc>=4)?atol(argv[3])*1024l*1024l*1024l:8l*1024l*1024l*1024l;
	long changeindex;

	Graph graph(path);
	graph.set_memory_bytes(memory_bytes);
	Bitmap * active_in = graph.alloc_bitmap();
	Bitmap * active_out = graph.alloc_bitmap();
	float *dist=(float *)malloc(sizeof(float)*graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * sizeof(VertexId) );

	active_in->clear();
	active_out->clear();
	active_out->set_bit(start_vid);

	std::fill(dist, dist+graph.vertices, (float)-1.);
	dist[start_vid] = 0;

	VertexId active_vertices = 1;

	double start_time = get_time();
	int iteration = 0;

	while (1) {

		iteration++;

		changeindex=0;
		std::swap(active_in, active_out);
		active_out->clear();

		active_vertices = graph.stream_edges<VertexId>([&](Edge & e){
			if ((dist[e.target]==-1)|(dist[e.source]+e.weight<dist[e.target])) {
				dist[e.target]=dist[e.source]+e.weight;
				active_out->set_bit(e.target);
				changeindex++;
			}
			return 0;
		}, active_in);

		if ((changeindex==0)|iteration==graph.vertices-1)
			break;

		fprintf(stderr,"progress: %d %d\r", iteration,changeindex);
		fflush(stderr);

	}
	double end_time = get_time();

	int discovered_vertices=0;
	#pragma omp parallel for reduction(+:discovered_vertices)
	for (i=0;i<graph.vertices;i++){
		if (dist[i]!=-1) discovered_vertices++;
	}
	fprintf(stderr,"discovered %d vertices from %d in %.2f seconds.\n", discovered_vertices, start_vid, end_time - start_time);

	for (i=0;i<graph.vertices;i++)
		if (dist[i]!=-1)
			printf("%d %f\n", i,dist[i]);

	free(dist);
*/

	return 0;
}
