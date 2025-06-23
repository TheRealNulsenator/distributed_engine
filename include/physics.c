#include "../lib/physics.h"
#include "../lib/render.h"

int physicsTicks = 2;

void physics_updateBody(Entity* entity, ArrayList* neighbors)
{
    entity->body.velocity = vec2_add(entity->body.velocity, vec2_scale(entity->body.acceleration, gtime.delta));
    for (int i = 0; i < physicsTicks; i++)
    {
        if(entity->body.collisionType >= 2)
        {
            physics_sweepResponse(entity, neighbors);
        }
        else
        {
            entity->body.aabb.position = vec2_add(entity->body.aabb.position, vec2_scale(entity->body.velocity, gtime.delta/physicsTicks));
        }
        if(entity->body.collisionType >= 1)
        {
            physics_staticResponse(entity, neighbors);
        }
    } 
}

Body physics_newBody(Vec2 pos, Vec2 rad, Vec2 vel, Vec2 acc, CollisionType type)
{
    Body newBody;
    newBody.aabb.position = (Vec2){pos.x, pos.y};
    newBody.aabb.radius = (Vec2){rad.x, rad.y};
    newBody.velocity = (Vec2){vel.x, vel.y};
    newBody.acceleration = (Vec2){acc.x, acc.y};
    newBody.collisionType = type;
    newBody.mass = 1;
    return newBody;
}

void aabb_MinMax(Vec2* resultMin, Vec2* resultMax, AABB box)
{
    Vec2 min, max;
    min = vec2_sub(box.position, box.radius);
    max = vec2_add(box.position, box.radius);
    memcpy(resultMin, &min, sizeof(Vec2));
    memcpy(resultMax, &max, sizeof(Vec2));
}

bool physics_point_intersect_body(Vec2 point, Body body)
{
    Vec2 min, max;
    aabb_MinMax(&min, &max, body.aabb);
    if (point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool physics_body_intersect_body(Body a, Body b)
{
    Vec2 min, max;
    aabb_MinMax(&min, &max, minkowski_difference(a, b));

    return (min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0);
}
AABB minkowski_difference(Body a, Body b)
{
    AABB result;
    result.position = vec2_sub(a.aabb.position, b.aabb.position);
    result.radius = vec2_add(a.aabb.radius, b.aabb.radius);
    return result;
}
Vec2 penetration_vector(AABB aabb)
{
    Vec2 min, max, result;
    aabb_MinMax(&min, &max, aabb);

    float min_dist = fabsf(min.x);
    result.x = min.x;
    result.y = 0;
    if(fabsf(max.x) < min_dist)
    {
        min_dist=fabsf(max.x);
        result.x = max.x;
    }
    if(fabsf(min.y) < min_dist)
    {
        min_dist=fabsf(min.y);
        result.x = 0;
        result.y = min.y;
    }
    if(fabsf(max.y)<min_dist)
    {
        result.x = 0;
        result.y = max.y;
    }
    return result;
}

Hit ray_intersect_aabb(Vec2 position, Vec2 magnitude, AABB aabb)
{
    Hit result;
	Vec2 min, max;
	aabb_MinMax(&min, &max, aabb);

	float last_entry = -INFINITY;
	float first_exit = INFINITY;

	float t1, t2;
    if (magnitude.x != 0) {
        t1 = (min.x - position.x) / magnitude.x;
        t2 = (max.x - position.x) / magnitude.x;

        last_entry = fmaxf(last_entry, fminf(t1, t2));
        first_exit = fminf(first_exit, fmaxf(t1, t2));
    } 
    else if (position.x <= min.x || position.x >= max.x) {
        return result;
    }
    if (magnitude.y != 0) {
        t1 = (min.y - position.y) / magnitude.y;
        t2 = (max.y - position.y) / magnitude.y;

        last_entry = fmaxf(last_entry, fminf(t1, t2));
        first_exit = fminf(first_exit, fmaxf(t1, t2));
    } 
    else if (position.y <= min.y || position.y >= max.y) {
        return result;
    }
	if (first_exit > last_entry && first_exit > 0 && last_entry < 1) {
		result.position.x = position.x + magnitude.x * last_entry;
		result.position.y = position.y + magnitude.y * last_entry;

		result.is_hit = true;
		result.time = last_entry;

		float dx = result.position.x - aabb.position.x;
		float dy = result.position.y - aabb.position.y;
		float px = aabb.radius.x - fabsf(dx);
		float py = aabb.radius.y- fabsf(dy);

		if (px < py) {
			result.normal.x = (dx > 0) - (dx < 0);
		} else {
			result.normal.y = (dy > 0) - (dy < 0);
		}
	}
    else
    {
        result.is_hit = false;
    }
	return result;
}

void physics_sweepResponse(Entity* entity, ArrayList* neighbors)
{
    Entity* other = NULL;
    Hit result = 
    {
        .time = 0xBEEF,
        .is_hit = false,
    };
    Vec2 scaledVelocity = vec2_scale(entity->body.velocity, gtime.delta/physicsTicks);
    if(vec2_mag(scaledVelocity) == 0)
    {
        return;
    }
    for(int i = 0; i < neighbors->length; i++)
    {
        Entity* neighbor = listGet(neighbors, i);
        if(neighbor->ID != entity->ID)
        {
            AABB sum = 
            {
                .position = neighbor->body.aabb.position,
                .radius = vec2_add(neighbor->body.aabb.radius, entity->body.aabb.radius)
            };
            Hit hit = ray_intersect_aabb(entity->body.aabb.position, scaledVelocity, neighbor->body.aabb);
            if(hit.is_hit)
            {
                if(hit.time < result.time)
                {
                    other = neighbor;
                    result = hit;
                }
                else if (hit.time == result.time)
                {
                    Vec2 velocity = entity->body.velocity;
                    if (fabsf(velocity.x) > fabsf(velocity.y) && hit.normal.x != 0)
                    {
                        other = neighbor;
                        result = hit;
                    }
                    else if (fabsf(velocity.y) > fabsf(velocity.x) && hit.normal.y != 0)
                    {
                        other = neighbor;       
                        result = hit;
                    }
                }
            }        
        }
    }
    if(result.is_hit)
    {
        entity->body.aabb.position = result.position;
        if(result.normal.y != 0)
        {
            entity->body.aabb.position.x += scaledVelocity.x;
        }
        if(result.normal.x != 0)
        {
            entity->body.aabb.position.y += scaledVelocity.y;
        }    
    }
    else 
    {
        entity->body.aabb.position = vec2_add(entity->body.aabb.position, scaledVelocity);
    }
}

void physics_staticResponse(Entity* entity, ArrayList* neighbors)
{
    for(int i = 0; i < neighbors->length; i++)
    {
        Entity* neighbor = listGet(neighbors, i);
        if(neighbor->ID != entity->ID)
        {
            AABB mink = minkowski_difference(neighbor->body, entity->body);
            Vec2 min, max;
            aabb_MinMax(&min, &max, mink);
            if(min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0)
            {
                Vec2 penetration = penetration_vector(mink);
                entity->body.aabb.position = vec2_add(entity->body.aabb.position, penetration);
                physics_inelasticCollision(entity, neighbor, penetration, 0.8);
            }
        }
    }
}

void physics_inelasticCollision(Entity* a, Entity* b, Vec2 normal, float restitution)
{
    if(a->body.collisionType >= 2)
    {
        float scalar = 1-(restitution/physicsTicks);
        if(b->body.collisionType >= 2)
        {
            float mA = a->body.mass;
            float mB = b->body.mass;
            float vAi, vAf;
            float vBi, vBf;
            if(normal.x != 0)
            {
                vAi = a->body.velocity.x;
                vBi = b->body.velocity.x;
                vAf = (((mA - mB)/(mA + mB))*vAi + ((2*mB)/(mA + mB))*vBi);
                vBf = (((mB - mA)/(mA + mB))*vBi + ((2*mA)/(mA + mB))*vAi);
                a->body.velocity.x = vAi + ((vAf - vAi)*restitution);
                b->body.velocity.x = vBi + ((vBf - vBi)*restitution);
            }
            if(normal.y != 0)
            {
                vAi = a->body.velocity.y;
                vBi = b->body.velocity.y;
                vAf = (((mA - mB)/(mA + mB))*vAi + ((2*mB)/(mA + mB))*vBi);
                vBf = (((mB - mA)/(mA + mB))*vBi + ((2*mA)/(mA + mB))*vAi);
                a->body.velocity.y = vAi + ((vAf - vAi)*restitution);
                b->body.velocity.y = vBi + ((vBf - vBi)*restitution);
            }
        }
        else
        {
            if(normal.x != 0)
            {
                a->body.velocity.x = -restitution * a->body.velocity.x;
            }
            if(normal.y != 0)
            {
                a->body.velocity.y = -restitution * a->body.velocity.y; 
            }
        } 
    }
}
