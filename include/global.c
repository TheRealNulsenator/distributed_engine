#include "../lib/global.h"


ArrayList *listCreate(size_t item_size)
{
	ArrayList *list = malloc(sizeof(ArrayList));
	if(!list)
	{
		ReturnError("Could not allocate memory for List\n");
	}
	list->item_size = item_size;
	list->capacity =  1;
	list->length = 0;
	list->items = malloc(item_size * list->capacity);
	if(!list->items)
	{
		ReturnError("Could not allocate memory for List\n");
	}
	return list;
}

size_t listAppend(ArrayList *list, void *newItem)
{
	if(list->length >= list->capacity)
	{
		list->capacity = list->length + 1;

		void *newItemsArray = realloc(list->items, list->item_size * list->capacity);
		if(!newItemsArray)
		{
			ReturnError("Could not allocate memory for List\n");
		}
		list->items = newItemsArray;
	}
	memcpy((uint8_t*)list->items + list->item_size * list->length, newItem, list->item_size);
	return list->length++;
}

void *listGet(ArrayList *list, size_t index)
{
	if(index >= list-> length)
	{
		ReturnError("Index out of bounds\n");
	}
	return (uint8_t*)list->items + index * list->item_size;
}

uint8_t listRemove(ArrayList *list, size_t index)
{
	if(list->length ==0)
	{
		ReturnError("List is empty\n");
	}
	if(index >= list-> length)
	{
		ReturnError("Index out of bounds\n");
	}
	if(list->length ==1)
	{
		list->length=0;
		return 0;
	}
	--list->length;
	uint8_t *item_ptr = (uint8_t*)list->items + index * list->item_size;
	uint8_t *end_ptr = (uint8_t*)list->items + list->length * list->item_size;
	memcpy(item_ptr, end_ptr, list->item_size);
	return 0;
}


int randomInt(int max) 
{
	return (rand() % max) + 1;
}



void ReturnError(char * msg) {
	printf("%s\n", msg);
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
	Vec2 result;
	result.x = a.x-b.x;
	result.y = a.y-b.y;
	return result;
}

Vec2 vec2_add(Vec2 a, Vec2 b)
{
	Vec2 result;
	result.x = a.x+b.x;
	result.y = a.y+b.y;
	return result;
}

Vec2 vec2_scale(Vec2 vector, float scalar)
{
	Vec2 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	return result;
}

float vec2_mag(Vec2 vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}