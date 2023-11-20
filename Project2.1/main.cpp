#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctype.h>
#include <cstring>
#include <cstdlib>
#include <limits.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
using namespace std;

typedef struct {
    int size;  // aktualny rozmiar wektora
    int capacity;  // pojemność wektora
    void** data;  // wskaźnik na tablicę przechowującą dane wektora
} Vector;

// Tworzenie nowego wektora
Vector* new_vector() {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    vector->size = 0;
    vector->capacity = 2;
    vector->data = (void**)malloc(sizeof(void*) * vector->capacity);
    return vector;
}

// Dodawanie nowego elementu do wektora
void vector_push(Vector* vector, void* value) {
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->data = (void**)realloc(vector->data, sizeof(void*) * vector->capacity);
    }
    vector->data[vector->size] = value;
    vector->size++;
}

// Usuwanie wektora i zwalnianie pamięci
void delete_vector(Vector* vector) {
    std::free(vector->data);
    std::free(vector);
}

void* vector_get(Vector* vector, int index) {
    if (index >= vector->size || index < 0) {
        printf("Index out of bounds!\n");
        return NULL;
    }
    return vector->data[index];
}
typedef struct {
    char* buffer;
    size_t length;
} String;

void init_string(String* str, const char* value) {
    size_t length = strlen(value);
    str->buffer = (char*)malloc(sizeof(char) * (length + 1));
    strncpy(str->buffer, value, length);
    str->buffer[length] = '\0';
    str->length = length;
}

void clear_string(String* str) {
    std::free(str->buffer);
    str->buffer = NULL;
    str->length = 0;
}

void appendChar(String* str, const char c) {
    char* newBuffer = (char*)realloc(str->buffer, str->length + 2);
    if (newBuffer == NULL) {
        return;
    }
    str->buffer = newBuffer;
    str->buffer[str->length] = c;
    str->buffer[str->length + 1] = '\0';
    str->length++;
}

void append_string(String* str, char* value) {
    size_t value_length = strlen(value);
    size_t new_length = str->length + value_length;
    str->buffer = (char*)realloc(str->buffer, sizeof(char) * (new_length + 1));
    strcat(str->buffer, value);
    str->length = new_length;
    str->buffer[str->length - 1] = '\0';
}
int compare_strings(String* str1, String* str2) {
    if (str1->length != str2->length) {
        return -1;
    }
    return strcmp(str1->buffer, str2->buffer);
}

void reverseString(String* str) {
    for (int i = 0; i < str->length / 2; i++) {
        char temp = str->buffer[i];
        str->buffer[i] = str->buffer[str->length - i - 1];
        str->buffer[str->length - i - 1] = temp;
    }
}
void trim(String* str) {
    int start = 0;
    int end = str->length - 1;
    while (start <= end && isspace(str->buffer[start])) {
        start++;
    }
    while (end >= start && isspace(str->buffer[end])) {
        end--;
    }
    for (int i = 0; i <= end - start; i++) {
        str->buffer[i] = str->buffer[start + i];
    }
    str->length = end - start + 1;
    str->buffer[str->length] = '\0';
}

struct Distance {
    int dist;
    int id;
};
struct City {
    String name;
    int x, y;
    int index;
    Vector* distances = new_vector();
    //Vector [Distance] distances;
};
struct Connection {
    City* source;
    City* destination;
    int time;
};
struct Query {
    City* source;
    City* destination;
    int type;
};
struct Map {
    char** board;
    int height, width;
};
struct Node {
    int x, y, time;
};
class CityHashMap {
public:
    CityHashMap(Vector* cities) {
        tableSize = cities->size * 2;
        table = new CityHashEntry * [tableSize];
        for (int i = 0; i < tableSize; i++) {
            table[i] = NULL;
        }
        for (int i = 0; i < cities->size; i++) {
            City* city = (City*)vector_get(cities, i );
            int index = city->index;
            String name = city->name;
            int hashValue = hash(name);
            CityHashEntry* entry = new CityHashEntry;
            entry->name = name;
            entry->index = index;
            entry->next = NULL;
            if (table[hashValue] == NULL) {
                table[hashValue] = entry;
            }
            else {
                CityHashEntry* current = table[hashValue];
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = entry;
            }
        }
    }
    int getCityIndex(String name) {
        int hashValue = hash(name);
        CityHashEntry* current = table[hashValue];
        while (current != NULL) {
            if (compare_strings(&current->name,&name) ==0) {
                return current->index;
            }
            current = current->next;
        }
        
        return -1;
    }
private:
    struct CityHashEntry {
        String name;
        int index;
        CityHashEntry* next;
    };
    CityHashEntry** table;
    int tableSize;
    int hash(String name) {
        unsigned long long hashValue = 5381;
        for (int i = 0; i < name.length; i++) {
            hashValue = ((hashValue << 5) + hashValue) + name.buffer[i];
        }
        return hashValue % tableSize;
    }

};

void readConnections(CityHashMap cityMap, int k, Vector& cities) {
    for (int i = 0; i < k; i++) {
        String sourceName;
        init_string(&sourceName, "");
        char c = getchar();
        if (c == '\n') {
            c = getchar();
        }
        while (!isspace(c)) {
            appendChar(&sourceName, c);
            c = getchar();
        }
        String destName;
        init_string(&destName, "");
        c = getchar();
        if (c == '\n') {
            c = getchar();
        }
        while (!isspace(c)) {
            appendChar(&destName, c);
            c = getchar();
        }
        int time = 0;
        c = getchar();
        if (c == '\n') {
            c = getchar();
        }
        while (isdigit(c)) {
            time = time * 10 + (c - '0');
            c = getchar();
        }
        int startIndex = cityMap.getCityIndex(sourceName);
        int endIndex = cityMap.getCityIndex(destName);
        Distance* distance = (Distance*)malloc(sizeof(Distance));
        distance->dist = time;
        distance->id = endIndex;
        City* city4 = (City*)vector_get(&cities, startIndex);
        vector_push(city4->distances, distance);
        //.push_back(distance);
    }
}

void dijkstra(int startIndex, int endIndex, Vector cities, int type) {
    int* queue = new int[cities.size + 10];
    int* distance = new int[cities.size + 10];
    int* prev = new int[cities.size + 10];
    bool* visited = new bool[cities.size + 10];
    Vector* paths = new_vector();
    //Vector** paths = new Vector * [cities.size];
    for (int i = 0; i < cities.size; i++) {
        distance[i] = INT_MAX;
        visited[i] = false;
        prev[i] = -1;
        //paths[i] = new_vector();
    }
    distance[startIndex] = 0;
    int front = 0;
    int rear = 0;
    queue[rear] = startIndex;
    visited[startIndex] = true;
    rear++;
    while (front != rear) {
        int current = queue[front];
        front++;
        visited[current] = false;
        City* city = (City*)vector_get(&cities, current);
        for (int i = 0; i < city->distances->size; i++) {
            int index_A = city->index;
            Distance distanceVec = *(Distance*)vector_get(city->distances, i);
            int index_B = distanceVec.id;
            int time = distanceVec.dist;
            if (distance[index_A] != INT_MAX && distance[index_A] + time < distance[index_B]) {
                distance[index_B] = distance[index_A] + time;
                prev[index_B] = index_A;
                //Vector* pathsVec = prev[index_B];
                //vector_push(pathsVec, &index_A);
                //for (int j = 0; j < paths[index_A]->size; j++) {
                  //  vector_push(pathsVec, vector_get(paths[index_A], j));
                //}
                if (!visited[index_B]) {
                    queue[rear++] = index_B;
                    visited[index_B] = true;
                }
            }
        }
    }
    if (type == 0) {
        std::cout << distance[endIndex] << std::endl;
    }
    if (type == 1) {
        std::cout << distance[endIndex] << " ";
        if (prev[endIndex] != -1) {
            int curr = endIndex;
            while (curr != startIndex) {
                City* city5 = (City*)vector_get(&cities, curr);
                vector_push(paths,city5);
                curr = prev[curr];
            }
            for (int i = paths->size - 1; i > 0; i--) {
                //int index = *(int*)vector_get(paths, i);
                City* city = (City*)vector_get(paths, i);
                std::cout << city->name.buffer << " ";
            }
            vector_push(paths, &endIndex);
        }
        std::cout << std::endl;
    }
    //for (int i = 0; i < cities.size; i++) {
        delete_vector(paths);
    
    //delete[] paths;
}


void readQueries(Vector& queries, int q, Vector& cities) {
    for (int i = 0; i < q; i++) {
        //Query* querie = (Query*)malloc(sizeof(Query));
        Query* querie = new Query;
        String sourceName;
        init_string(&sourceName, "");
        City* source = (City*)malloc(sizeof(City));
        char c = getchar();
        if (c == '\n') {
            c = getchar();
        }
        while (!isspace(c)) {
            appendChar(&sourceName, c);
            c = getchar();
        }
        source->name = sourceName;
        City* destination = (City*)malloc(sizeof(City));
        String destName;
        init_string(&destName, "");
        c = getchar();
        if (c == '\n') {
            c = getchar();
        }
        while (!isspace(c)) {
            appendChar(&destName, c);
            c = getchar();
        }
        destination->name = destName;
        int type = 0;
        c = getchar();
        if (c == '\n') {
            c = getchar();
        }
        while (isdigit(c)) {
            type = type * 10 + (c - '0');
            c = getchar();
        }
        querie->source = source;
        querie->destination = destination;
        querie->type = type;
        vector_push(&queries, querie);
    }
    for (int i = 0; i < queries.size; i++) {
        int startIndex = -1;
        int endIndex = -1;
        for (int j = 0; j < cities.size; j++) {
            Query* query1 = (Query*)vector_get(&queries, i);
            City* city = (City*)vector_get(&cities, j);
            if (compare_strings(&query1->source->name, &city->name) == 0) {
                startIndex = j;
            }
            if (compare_strings(&query1->destination->name, &city->name) == 0) {
                endIndex = j;
            }
        }
        if (startIndex == -1 || endIndex == -1) {
            continue;
        }
        else {
            Query* query2 = (Query*)vector_get(&queries,i);
            switch (query2->type) {
            case 0:
                dijkstra(startIndex, endIndex, cities, 0);
                break;
            case 1:
                dijkstra(startIndex, endIndex, cities, 1);
                break;
            }
        }
    }
}
void bubbleSort(Node arr[], int n) {
    bool swaped;
    for (int i = 0; i < n - 1; i++) {
        swaped = false;
        for (int j = 0; j < n - i; j++) {
            if (arr[j].time < arr[j + 1].time) {
                swap(arr[j], arr[j + 1]);
                swaped = true;
            }
            if (!swaped) break;
        }
    }
}
int bfs(int startx, int starty, int endx, int endy, bool** visit, Map map) {
    if (startx == endx && starty == endy)
        return 0;
    int rear = -1;
    int front = 0;
    Vector* queue = new_vector();
    Node start;
    start.x = startx;
    start.y = starty;
    start.time = 0;
    for (int i = 0; i < map.height; i++)
        for (int j = 0; j < map.width; j++)
            visit[i][j] = false;
    visit[starty][startx] = true;
    vector_push(queue, &start);
    rear++;
    while (front <= rear) {
        Node current = *(Node*)vector_get(queue, front);
        front++;
        Node neighbors[4];
        int tempX[4] = { 0,-1,0,1 };
        int tempY[4] = { -1,0,1,0 };
        for (int k = 0; k < 4; k++) {
            if ((current.x + tempX[k] >= 0 && current.x + tempX[k] < map.width) && (current.y + tempY[k] >= 0 && current.y + tempY[k] < map.height)) {
                neighbors[k].x = current.x + tempX[k];
                neighbors[k].y = current.y + tempY[k];
                if (map.board[neighbors[k].y][neighbors[k].x] == '#' && !visit[neighbors[k].y][neighbors[k].x]) {
                    visit[neighbors[k].y][neighbors[k].x] = true;
                    neighbors[k].time = ((endx - neighbors[k].x) * (endx - neighbors[k].x)) + ((endy - neighbors[k].y) * (endy - neighbors[k].y));

                }
                else if (neighbors[k].x == endx && neighbors[k].y == endy) {
                    return current.time + 1;
                }
                else
                    neighbors[k].time = -1;
            }
            else
                neighbors[k].time = -1;
        }
        bubbleSort(neighbors, 4);
        for (int k = 0; k < 4; k++) {
            if (neighbors[k].time != -1) {
                Node* neighbor = (Node*)malloc(sizeof(Node));
                neighbor->x = neighbors[k].x;
                neighbor->y = neighbors[k].y;
                neighbor->time = current.time + 1;
                vector_push(queue, neighbor);
                rear++;
            }
        }
    }
    return -1;
}
void findRout(Vector* cities, Map map, bool** visit) {
    bool skip = true;
    for (int i = 0; i < cities->size; i++) {
        int dx[4] = { -1, 0, 1, 0 };
        int dy[4] = { 0, -1, 0, 1 };
        for (int t = 0; t < 4; t++) {
            City* city;
            city = (City*)vector_get(cities, i);
            int x = city->x + dx[t];
            int y = city->y + dy[t];
            if (x < 0 || x >= map.width || y < 0 || y >= map.height) continue;
            if (map.board[y][x] == '*' || map.board[y][x] == '#') skip = false;
        }
        if (skip) continue;
        for (int j = i; j < cities->size; j++) {
            for (int r = 0; r < 4; r++) {
                City* city2;
                city2 = (City*)vector_get(cities, j);
                int x = city2->x + dx[r];
                int y = city2->y + dy[r];
                if (x < 0 || x >= map.width || y < 0 || y >= map.height) continue;
                if (map.board[y][x] == '*' || map.board[y][x] == '#') skip = false;
            }
            City* cityStart = (City*)vector_get(cities, i);
            City* cityEnd = (City*)vector_get(cities, j);
            int dest = bfs(cityStart->x, cityStart->y, cityEnd->x, cityEnd->y, visit, map);
            if (dest != -1) {
                Distance* dist = (Distance*)malloc(sizeof(Distance));
                dist->dist = dest;
                dist->id = j;
                City* city3 = (City*)vector_get(cities, i);
                vector_push(city3->distances, dist);
                Distance* distReverse = (Distance*)malloc(sizeof(Distance));
                distReverse->dist = dest;
                distReverse->id = i;
                City* city1 = (City*)vector_get(cities, j);
                vector_push(city1->distances, distReverse);
            }
        }
    }
}

int main() {
    int w, s, k = 0, q = 0, l = 0;
    bool isEnd = false;
    Vector* cities = new_vector();
    Map* map = (Map*)malloc(sizeof(Map));
    map->width = 10;
    map->height = 10;
    map->board = (char**)malloc(map->height * sizeof(char*));
    for (int i = 0; i < map->height; i++) {
        map->board[i] = (char*)malloc(map->width * sizeof(char));
        for (int j = 0; j < map->width; j++) {
            map->board[i][j] = 0;
        }
    }
    std::cin >> map->width >> map->height;
    map->board = new char* [map->height];
    for (int i = 0; i < map->height + 4; i++) {
        map->board[i] = new char[map->width];
        if (i > 2) {
            l = i - 3;
        }
        for (int j = 0; j < map->width; j++) {
            if (i < map->height) {
                char a = getchar();
                if (isspace(a)) {
                    j--;
                    continue;
                }
                map->board[i][j] = a;
            }
            if (map->board[l][j] == '*') {
                City* city = new City;
                init_string(&(city->name), "");
                city->x = j;
                city->y = l;
                bool isNameFind = false;
                if (i > 2) {
                    for (int y = l - 1; y <= l + 1; y++) {
                        for (int x = j - 1; x <= j + 1; x++) {
                            if (y >= 0 && y < map->height && x >= 0 && x < map->width) {
                                if (isalnum(map->board[y][x])) {
                                    if (x - 1 >= 0 && x + 1 < map->width) {
                                        if (isalnum(map->board[y][x - 1]) && isalnum(map->board[y][x + 1])) continue;
                                    }
                                    isEnd = false;
                                    int t = x;
                                    if (t == (map->width - 1)) {
                                        isEnd = true;
                                    }
                                    if (t != 0) {
                                        if (isalnum(map->board[y][t - 1])) {
                                            isEnd = true;
                                        }
                                    }
                                    while (t >= 0 && t < map->width && isalnum(map->board[y][t])) {
                                        appendChar(&(city->name), (map->board[y][t]));
                                        if (isEnd) t--;
                                        else t++;
                                    }
                                    isNameFind = true;
                                }
                            }
                            if (isNameFind) break;
                        }
                        if (isNameFind) break;
                    }
                    city->index = cities->size;
                    if (isEnd) {
                        reverseString(&(city->name));
                    }
                    trim(&city->name);
                    vector_push(cities, (City*)city);
                }
            }
        }
    }
    CityHashMap cityMap(cities);
    bool** visit = (bool**)malloc(map->height * sizeof(bool*));
    for (int i = 0; i < map->height; i++) {
        visit[i] = (bool*)malloc(map->width * sizeof(bool));
        for (int j = 0; j < map->width; j++) {
            visit[i][j] = false;
        }
    }
    findRout(cities, *map, visit);
    std::cin >> k;
    readConnections(cityMap, k, *cities);
    std::cin >> q;
    Vector* queries = new_vector();
    readQueries(*queries, q, *cities);
    //std::free(map);
    return 0;
}